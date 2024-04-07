#pragma once

#include "basic_option.hpp"

namespace iridium::rclone::option::performance
{
	/**
	 * @brief In memory buffer size when reading files for each --transfer (default 16Mi)
	 * @return performance
	 */
	static auto buffer_size(const std::string& value) -> uptr_basic_opt
	{
		return std::make_unique<basic_option>("--buffer-size", value);
	};

	/**
	 * @brief Number of checkers to run in parallel (default 8)
	 * @return performance
	 */
	static auto checkers(int value) -> uptr_basic_opt
	{
		return std::make_unique<basic_option>("--checkers", std::to_string(value));
	};

	/**
	 * @brief Number of file transfers to run in parallel (default 4)
	 * @return performance
	 */
	static auto transfers(int value) -> uptr_basic_opt
	{
		return std::make_unique<basic_option>("--transfers", std::to_string(value));
	};
} // namespace iridium::rclone::option
