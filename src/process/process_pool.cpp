#include <process_pool.hpp>
#include <boost/thread.hpp>
#include <vector>
#include <map>
#include <condition_variable>
#include <atomic>

namespace iridium::rclone
{
	class process_pool::_process_pool_impl
	{
	public:
		std::map<priority, std::vector<std::unique_ptr<process>>, std::greater<>> _processes;
		uint16_t _simultaneous_processes{};
		std::atomic<uint16_t> _running_processes = 0;
		std::atomic<uint16_t> _executed_processes = 0;

		boost::thread _thread;
		std::mutex _process_mutex;
		std::condition_variable _cv_process;

		std::mutex _mutex;
		std::condition_variable _cv;
		std::mutex _wait_mutex;
		std::condition_variable _wait_cv;

		enum class operation : uint8_t
		{
			none,
			lock,
		};

		enum state
		{
			running,
			stopped,
		};

		state _state = running;
		std::atomic<operation> _operation = operation::none;

		explicit _process_pool_impl(uint16_t simultaneous_processes) : _simultaneous_processes(simultaneous_processes)
		{
			_thread = boost::thread([this]
			{
				while (true)
				{
					std::unique_lock lock(_process_mutex);
					_cv_process.wait(lock, [this]
					{
						boost::this_thread::interruption_point();
						return _running_processes < _simultaneous_processes and get_process() not_eq nullptr;
					});
					boost::this_thread::interruption_point();
					auto *process = get_process();
					if (process == nullptr) { continue; }
					process->on_finish([this](int)
					{
						_running_processes--;
						_executed_processes++;
						_cv_process.notify_one();
						_wait_cv.notify_one();
					});
					process->execute();
					_running_processes++;
				}
			});
		}

		auto add_process(std::unique_ptr<process> &&process, process_pool::priority priority) -> void
		{
			lock();
			if (_state == stopped)
				throw std::runtime_error("process pool is stopped");
			_processes[priority].push_back(std::move(process));
			_cv_process.notify_one();
			unlock();
		}

		auto add_process(process *process, process_pool::priority priority) -> void
		{
			add_process(process_uptr(process), priority);
		}

		void stop()
		{
			lock();
			_thread.interrupt();
			_cv_process.notify_one();
			for (const auto &pair: _processes)
				for (const auto &process: pair.second)
					if (process->is_running())
						process->stop();
			_state = stopped;
			unlock();
		}

		void stop_all_processes()
		{
			lock();
			for (const auto &pair: _processes)
				for (const auto &process: pair.second)
					if (process->is_running())
						process->stop();
			_running_processes = 0;
			_executed_processes = 0;
			unlock();
		}

		void wait()
		{
			auto unique_lock = std::unique_lock(_wait_mutex);
			_wait_cv.wait(unique_lock, [this]
			{
				auto size = 0;
				for (const auto &pair: _processes)
					size += pair.second.size();
				return _executed_processes == size;
			});
		}

		auto empty() const -> bool { return _processes.empty(); }

		auto size() const -> uint16_t { return _processes.size(); }

		void clear_pool()
		{
			lock();
			_processes.clear();
			unlock();
		}

		auto set_simultaneous_processes(uint16_t simultaneous_processes) -> void
		{
			_simultaneous_processes = simultaneous_processes;
		}

		void stop_all_processes_and_clear()
		{
			stop_all_processes();
			clear_pool();
		}

		~_process_pool_impl() { stop(); }

		void lock()
		{
			std::unique_lock lock(_mutex);
			_cv.wait(lock, [this] { return _operation == operation::none; });
			_operation = operation::lock;
		}

		void unlock()
		{
			_operation = operation::none;
			_cv.notify_one();
		}

		auto get_process() -> process *
		{
			lock();
			process *result = nullptr;
			for (const auto &pair: _processes)
				for (const auto &process: pair.second)
					if (process->get_state() == process::state::not_launched)
						result = process.get();
			unlock();
			return result;
		}

		auto nb_running_processes() -> uint16_t
		{
			lock();
			_running_processes = 0;
			for (const auto &pair: _processes)
				for (const auto &process: pair.second)
					if (process->is_running())
						_running_processes++;
			unlock();
			return _running_processes;
		}
	};

	process_pool::~process_pool()
	{
		delete _impl;
	}

	process_pool::process_pool(uint16_t simultaneous_processes)
	{
		_impl = new _process_pool_impl(simultaneous_processes);
	}

	process_pool::process_pool(process_pool &&pool) noexcept
	{
		_impl = std::move(pool._impl);
		pool._impl = nullptr;
	}

	auto process_pool::operator=(process_pool &&pool) noexcept -> process_pool&
	{
		_impl = std::move(pool._impl);
		pool._impl = nullptr;
		return *this;
	}

	auto process_pool::add_process(std::unique_ptr<process> &&process, priority priority) const -> void
	{
		_impl->add_process(std::move(process), priority);
	}

	auto process_pool::add_process(process *process, priority priority) const -> void
	{
		add_process(process_uptr(process), priority);
	}

	void process_pool::stop() const { _impl->stop(); }

	void process_pool::stop_all_processes() const { _impl->stop_all_processes(); }

	void process_pool::wait() const { _impl->wait(); }

	auto process_pool::empty() const -> bool { return _impl->empty(); }

	auto process_pool::size() const -> uint16_t { return _impl->size(); }

	void process_pool::clear_pool() const { _impl->clear_pool(); }

	auto process_pool::set_simultaneous_processes(uint16_t simultaneous_processes) const -> void
	{
		_impl->set_simultaneous_processes(simultaneous_processes);
	}

	void process_pool::stop_all_processes_and_clear() const
	{
		stop_all_processes();
		clear_pool();
	}
}; // namespace iridium::rclone
