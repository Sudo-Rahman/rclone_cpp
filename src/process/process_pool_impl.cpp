#include <process_pool.hpp>
#include <boost/thread.hpp>
#include <vector>
#include <map>
#include <condition_variable>
#include <atomic>
#include <thread>
#include <numeric>
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

        std::thread _thread;
        std::mutex _process_mutex;
        std::condition_variable _cv_process;

        std::mutex _mutex;
        std::mutex _wait_mutex;
        std::condition_variable _wait_cv;

        uint32_t _process_counter = 0;

        state _state = not_launched;

        explicit _process_pool_impl_(uint16_t simultaneous_processes) : _simultaneous_processes(simultaneous_processes)
        {
            start_thread();
        }

        void start_thread(){
            _thread = std::thread([this]
                                  {
                                      _state = running;
                                      while (_state == running)
                                      {
                                          {
                                              std::unique_lock lock(_process_mutex);
                                              _cv_process.wait(lock, [this]
                                              {
                                                  if (_state == stopped)
                                                      return true;
                                                  std::lock_guard lock(_mutex);
                                                  return (_running_processes < _simultaneous_processes && get_process() != nullptr) ||
                                                         _state == stopped;
                                              });
                                          }
                                          if (_state == stopped)
                                              break;
                                          std::lock_guard lock(_mutex);
                                          auto* process = get_process();
                                          if (process == nullptr) { continue; }
                                          process->on_finish([this]
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
            while (_state != running)
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        auto add_process(std::unique_ptr<process> &&process, priority priority) -> void
        {
            {
                std::lock_guard lock(_mutex);
                if (_state == stopped)
                    throw std::runtime_error("process pool is stopped");
                _processes[priority].push_back(std::move(process));
                _process_counter++;
            }
            _cv_process.notify_one();
        }

        void stop()
        {
            if (_state == stopped)
                return;
            {
                std::lock_guard lock(_mutex);
                _state = stopped;
                for (const auto &pair: _processes)
                    for (const auto &process: pair.second)
                        if (process->is_running())
                            process->stop();
            }
            _cv_process.notify_one();
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


        auto set_simultaneous_processes(uint16_t simultaneous_processes) -> void
        {
            _simultaneous_processes = simultaneous_processes;
            _cv_process.notify_one();
        }

        void stop_all_processes_and_clear()
        {
            std::lock_guard lock(_mutex);
            for (const auto &pair: _processes)
                for (const auto &process: pair.second)
                    if (process->is_running())
                        process->stop();
            _processes.clear();
            _running_processes = 0;
            _executed_processes = 0;
            _process_counter = 0;
        }

        ~_process_pool_impl_()
        {
            stop();
            if (_thread.joinable()){
                _thread.join();
            }
        }

        auto get_process() -> process *
        {
            for (const auto &pair: _processes)
                for (const auto &process: pair.second)
                    if (process->get_state() == process::state::not_launched)
                        return process.get();
            return nullptr;
        }
    };
}
