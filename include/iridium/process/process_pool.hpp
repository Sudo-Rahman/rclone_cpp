#pragma once

#include "process.hpp"
#include <boost/thread.hpp>


namespace iridium::rclone
{
    class process_pool {
        std::vector<std::unique_ptr<process>> _processes;
        size_t _simultaneous_processes{};
        std::atomic<size_t> _running_processes = 0;


        boost::thread _thread;
        std::mutex _process_mutex;
        std::condition_variable _cv_process;

        std::mutex _mutex;
        std::condition_variable _cv;

        process_pool() = default;

        enum class operation : uint8_t
        {
            none,
            lock,
        };
        std::atomic<operation> _operation = operation::none;

        void lock();

        void unlock();

        auto get_process() -> process *;

        auto executed_processes() -> size_t;

    public:

        /// @brief Construct a process pool with a maximum number of simultaneous processes
        explicit process_pool(size_t simultaneous_processes);

        ~process_pool();


        process_pool(const process_pool &) = delete;

        process_pool(process_pool &&) = delete;

        auto operator=(const process_pool &) -> process_pool & = delete;

        auto operator=(process_pool &&) -> process_pool & = delete;

        /// @brief Add a process to the pool
        auto add_process(std::unique_ptr<process> &&process) -> void;

        void clear_pool();

        /// @brief Get the number of processes in the pool
        [[nodiscard]] std::size_t size() const;

        /// @brief Check if the pool is empty
        [[nodiscard]] bool empty() const;

        /// @brief Wait for all processes to finish
        void wait();

        /// @brief stop all processes
        void stop();

        /// @brief stop all processes and clear the pool
        void stop_and_clear();
    };
} // namespace iridium::rclone