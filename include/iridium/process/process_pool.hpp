#pragma once

#include "process.hpp"
#include <boost/thread.hpp>


namespace iridium::rclone
{
	class process_pool
	{
	public:
		enum priority
		{
			low,
			normal,
			high,
			max
		};

		process_pool() = delete;

		/// @brief Construct a process pool with a maximum number of simultaneous processes
		explicit process_pool(uint16_t simultaneous_processes);

		~process_pool();

		process_pool(const process_pool&) = delete;

		process_pool(process_pool&&) = delete;


		auto operator=(const process_pool&) -> process_pool& = delete;

		auto operator=(process_pool&&) -> process_pool& = delete;

		/**
		 * @brief Add a process to the pool
		 * @param process
		 */
		auto add_process(std::unique_ptr<process>&& process, priority = normal) -> void;

		auto add_process(process * process, priority = normal) -> void;

		/**
		 * @brief Clear all processes from the pool
		 */
		void clear_pool();

		auto set_simultaneous_processes(uint16_t simultaneous_processes) -> void;

		/**
		 * @brief Get the number of processes in the pool
		 * @return the number of processes in the pool
		 */
		[[nodiscard]] uint16_t size() const;

		/**
		 * @brief Check if the pool is empty
		 * @return true if the pool is empty
		 */
		[[nodiscard]] bool empty() const;

		/**
		 * @brief wait for all processes to finish
		 */
		void wait();

		/**
		 * @brief stop pool and all processes
		 */
		void stop();

		/**
		 * @brief stop all processes
		 */
		void stop_all_processes();

		/**
		 * @brief stop all processes and clear the pool
		 */
		void stop_all_processes_and_clear();

	private:
		std::map<priority, std::vector<std::unique_ptr<process>>,std::greater<>> _processes;
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

		void lock();

		void unlock();

		auto get_process() -> process*;

		auto nb_running_processes() -> uint16_t;
	};
} // namespace iridium::rclone
