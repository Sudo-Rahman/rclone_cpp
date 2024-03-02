#include <process_pool.hpp>
#include <iostream>


namespace iridium::rclone
{

    process_pool::process_pool(size_t simultaneous_processes)
            : _simultaneous_processes(simultaneous_processes)
    {
        _thread = boost::thread([this]
                                {
                                    while (true) {
                                        std::unique_lock<std::mutex> lock(_process_mutex);
                                        _cv_process.wait(lock, [this]
                                        {
                                            boost::this_thread::interruption_point();
                                            return _running_processes < _simultaneous_processes
                                                   and executed_processes() < _processes.size();
                                        });
                                        boost::this_thread::interruption_point();
                                        process_pool::lock();
                                        auto process = get_process();
                                        if (process == nullptr) {
                                            process_pool::unlock();
                                            continue;
                                        }
                                        process->on_finish([this](int)
                                                           {
                                                               _running_processes--;
                                                               _cv_process.notify_one();
                                                           });
                                        process->execute();
                                        _running_processes++;
                                        process_pool::unlock();
                                    }
                                });
    }


    auto process_pool::add_process(std::unique_ptr<process> &&process) -> void
    {
        lock();
        _processes.push_back(std::move(process));
        _cv_process.notify_one();
        unlock();
    }


    void process_pool::stop()
    {
        lock();
        _thread.interrupt();
        _cv_process.notify_one();
        for (auto &process: _processes)
            if (process->is_running())
                process->stop();
        unlock();
    }

    void process_pool::wait()
    {
        lock();
        for (auto &process: _processes)
            if (process->is_running())
                process->wait_for_finish();
        unlock();
    }

    bool process_pool::empty() const
    {
        return _processes.empty();
    }

    std::size_t process_pool::size() const
    {
        return _processes.size();
    }

    void process_pool::clear_pool()
    {
        lock();
        _processes.clear();
        unlock();
    }

    void process_pool::stop_and_clear()
    {
        stop();
        clear_pool();
    }

    process_pool::~process_pool()
    {
        stop();
        wait();
    }

    void process_pool::lock()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _cv.wait(lock, [this] { return _operation == operation::none; });
        _operation = operation::lock;
    }

    void process_pool::unlock()
    {
        _operation = operation::none;
        _cv.notify_one();
    }

    auto process_pool::get_process() -> process *
    {
        for (auto &process: _processes)
            if (process->get_state() == process::state::not_launched)
                return process.get();
        return nullptr;
    }

    auto process_pool::executed_processes() -> size_t
    {
        size_t count = 0;
        for (auto &process: _processes)
            if (not process->is_running() and process->get_state() not_eq process::state::not_launched)
                count++;
        return count;
    }

}; // namespace iridium::rclone