#include <process_pool.hpp>
#include <boost/thread.hpp>
#include <vector>
#include <map>
#include <condition_variable>
#include <atomic>
#include <thread>
#include <mutex>
#include <stdexcept>

namespace iridium::rclone
{
	class process_pool::_process_pool_impl_
	{
		friend process_pool;
		std::map<priority, std::vector<std::unique_ptr<process>>, std::greater<>> _processes;
		uint16_t _simultaneous_processes{};
		std::atomic<uint16_t> _running_processes = 0;
		std::atomic<uint16_t> _executed_processes = 0;

		boost::thread _thread;
		std::mutex _process_mutex;
		std::condition_variable _cv_process;

		std::mutex _mutex;
		std::mutex _wait_mutex;
		std::condition_variable _wait_cv;

		enum class operation
		{
			none,
			lock,
		};

		state _state = running;
		std::atomic<operation> _operation = operation::none;

		explicit _process_pool_impl_(uint16_t simultaneous_processes) : _simultaneous_processes(simultaneous_processes)
		{
			_thread = boost::thread([this]
			{
				while (_state == running)
				{
					std::unique_lock lock(_process_mutex);
					_cv_process.wait(lock, [this]
					{
						boost::this_thread::interruption_point();
						return _running_processes < _simultaneous_processes && get_process() != nullptr || _state != running;
					});
					if (_state != running)
						break;
					boost::this_thread::interruption_point();
					join_finished_processes();
					auto* process = get_process();
					if (process == nullptr) { continue; }
					process->on_finish([this](int)
					{
						_running_processes--;
						_executed_processes++;
						_cv_process.notify_one();
						_wait_cv.notify_one();
					});
					process->on_stop([this]
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
			{
				std::lock_guard lock(_mutex);
				if (_state == stopped)
					throw std::runtime_error("process pool is stopped");
				_processes[priority].push_back(std::move(process));
			}
			_cv_process.notify_one();
		}

		void stop()
		{
			{
				std::lock_guard lock(_mutex);
				_state = stopped;
				_thread.interrupt();
				for (const auto &pair: _processes)
					for (const auto &process: pair.second)
						if (process->is_running())
							process->stop();
			}
			_cv_process.notify_all();
		}

		void stop_all_processes()
		{
			std::lock_guard lock(_mutex);
			for (const auto &pair: _processes)
				for (const auto &process: pair.second)
					if (process->is_running())
						process->stop();
			_running_processes = 0;
		}

		void wait()
		{
			std::unique_lock lock(_wait_mutex);
			_wait_cv.wait(lock, [this]
			{
				auto size = 0;
				for (const auto &pair: _processes)
					size += pair.second.size();
				return _executed_processes == size;
			});
		}

		void clear_pool()
		{
			std::lock_guard lock(_mutex);
			_processes.clear();
			_running_processes = 0;
			_executed_processes = 0;
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

		~_process_pool_impl_()
		{
			stop();
			_thread.join();
		}

		auto get_process() -> process *
		{
			std::lock_guard lock(_mutex);
			for (const auto &pair: _processes)
				for (const auto &process: pair.second)
					if (process->get_state() == process::state::not_launched)
						return process.get();
			return nullptr;
		}

		auto join_finished_processes() -> void
		{
			std::lock_guard lock(_mutex);
			for (const auto &pair: _processes)
				for (const auto &process: pair.second)
					if (process->get_state() == process::state::finished)
						process->join();
		}
	};
}
