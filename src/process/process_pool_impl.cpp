#include <process_pool.hpp>
#include <boost/thread.hpp>
#include <vector>
#include <map>
#include <condition_variable>
#include <atomic>

namespace iridium::rclone
{
	class process_pool::_process_pool_impl_
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

		explicit _process_pool_impl_(uint16_t simultaneous_processes) : _simultaneous_processes(simultaneous_processes)
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

		auto add_process(std::unique_ptr<process> &&process, priority priority) -> void
		{
			lock();
			if (_state == stopped)
				throw std::runtime_error("process pool is stopped");
			_processes[priority].push_back(std::move(process));
			_cv_process.notify_one();
			unlock();
		}

		auto add_process(process *process, priority priority) -> void { add_process(process_uptr(process), priority); }

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

		~_process_pool_impl_() { stop(); }

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
}
