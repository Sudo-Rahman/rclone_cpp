#include <process_pool.hpp>


namespace iridium::rclone
{
	process_pool::process_pool(uint16_t simultaneous_processes)
		: _simultaneous_processes(simultaneous_processes)
	{
		_thread = boost::thread([this]
		{
			while (true)
			{
				std::unique_lock lock(_process_mutex);
				_cv_process.wait(lock, [this]
				{
					boost::this_thread::interruption_point();
					return nb_running_processes() < _simultaneous_processes
					       and not _processes.empty() and _executed_processes < _processes.size() and get_process()
					       not_eq nullptr;
				});
				boost::this_thread::interruption_point();
				auto * process = get_process();
				if (process == nullptr) { continue; }
				process->on_finish([this](int)
				{
					_running_processes--;
					_executed_processes++;
					_cv_process.notify_one();
				});
				process->execute();
				_running_processes++;
			}
		});
	}


	auto process_pool::add_process(std::unique_ptr<process>&& process, priority priority) -> void
	{
		for (const auto& pair: _processes)
			std::cout << pair.first << std::endl;
		std::cout << std::endl;
		lock();
		if (_state == stopped)
			throw std::runtime_error("process pool is stopped");
		_processes[priority].push_back(std::move(process));
		_cv_process.notify_one();
		unlock();
	}

	auto process_pool::add_process(process * process, priority priority) -> void
	{
		add_process(process_uptr(process), priority);
	}


	void process_pool::stop()
	{
		lock();
		_thread.interrupt();
		_cv_process.notify_one();
		for (const auto& pair: _processes)
			for (const auto& process: pair.second)
				if (process->is_running())
					process->stop();
		_state = stopped;
		unlock();
	}

	void process_pool::stop_all_processes()
	{
		lock();
		for (const auto& pair: _processes)
			for (const auto& process: pair.second)
				if (process->is_running())
					process->stop();
		_running_processes = 0;
		_executed_processes = 0;
		unlock();
	}

	void process_pool::wait()
	{
		lock();
		for (const auto& pair: _processes)
			for (const auto& process: pair.second)
				if (process->is_running())
					process->wait_for_finish();
		unlock();
	}

	auto process_pool::empty() const -> bool { return _processes.empty(); }

	auto process_pool::size() const -> uint16_t { return _processes.size(); }

	void process_pool::clear_pool()
	{
		lock();
		_processes.clear();
		unlock();
	}

	auto process_pool::set_simultaneous_processes(uint16_t simultaneous_processes) -> void
	{
		_simultaneous_processes = simultaneous_processes;
	}

	void process_pool::stop_all_processes_and_clear()
	{
		stop_all_processes();
		clear_pool();
	}

	process_pool::~process_pool() { stop(); }

	void process_pool::lock()
	{
		std::unique_lock lock(_mutex);
		_cv.wait(lock, [this] { return _operation == operation::none; });
		_operation = operation::lock;
	}

	void process_pool::unlock()
	{
		_operation = operation::none;
		_cv.notify_one();
	}

	auto process_pool::get_process() -> process*
	{
		lock();
		process * result = nullptr;
		for (const auto& pair: _processes)
			for (const auto& process: pair.second)
				if (process->get_state() == process::state::not_launched)
					result = process.get();
		unlock();
		return result;
	}

	auto process_pool::nb_running_processes() -> uint16_t
	{
		lock();
		_running_processes = 0;
		for (const auto& pair: _processes)
			for (const auto& process: pair.second)
				if (process->is_running())
					_running_processes++;
		unlock();
		return _running_processes;
	}
}; // namespace iridium::rclone
