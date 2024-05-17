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

		auto add_process(process * process, priority = normal) const-> void;

		/**
		 * @brief Clear all processes from the pool
		 */
		void clear_pool() const;

		auto set_simultaneous_processes(uint16_t simultaneous_processes)const -> void;

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

		/**
		 * @brief stop all processes
		 */
		void stop_all_processes() const;

		/**
		 * @brief stop all processes and clear the pool
		 */
		void stop_all_processes_and_clear() const;

	protected:

		class _process_pool_impl;
		_process_pool_impl *_impl;

	};
} // namespace iridium::rclone
