#pragma once

#include "process.hpp"
#include <memory>

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

		enum state
		{
			running,
			stopped,
		};

		process_pool() = delete;

		~process_pool();

		/// @brief Construct a process pool with a maximum number of simultaneous processes
		explicit process_pool(uint16_t simultaneous_processes);

		process_pool(const process_pool&) = delete;

		process_pool(process_pool&&) noexcept;

		auto operator=(const process_pool&) -> process_pool& = delete;

		auto operator=(process_pool&&)noexcept -> process_pool&;

		/**
		 * @brief Add a process to the pool
		 * @param process
		 */
		auto add_process(std::unique_ptr<process>&& process, priority = normal) const -> void;


		/**
		 * @brief Clear all processes from the pool
		 */
		void clear_pool() const;

		auto set_simultaneous_processes(uint16_t simultaneous_processes)const -> void;

		auto simultaneous_processes() const -> uint16_t;

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
		void wait() const;

		/**
		 * @brief stop pool and all processes
		 */
		void stop() const;

		[[nodiscard]] auto get_state() const -> state;

		/**
		 * @brief stop all processes
		 */
		void stop_all_processes() const;

		/**
		 * @brief stop all processes and clear the pool
		 */
		void stop_all_processes_and_clear() const;

		auto finished_processes() const -> uint16_t;

	protected:

		class _process_pool_impl_;
		_process_pool_impl_ *_impl;

	};
} // namespace iridium::rclone
