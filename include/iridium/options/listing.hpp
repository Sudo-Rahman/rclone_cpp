#pragma once

#include "basic_option.hpp"

namespace iridium::rclone::option::listing
{
	/**
	 * @brief Use recursive list if available; uses more memory but fewer transactions
	 * @return basic_option
	 */
	static auto fast_list() -> basic_opt_uptr { return std::make_unique<basic_option>("--fast-list"); }

	/**
	 * @brief Time to show if modtime is unknown for files and directories (default 2000-01-01T00:00:00Z)
	 * @return basic_option
	 */
	static auto default_time(const std::string& value) -> basic_opt_uptr
	{
		return std::make_unique<basic_option>("--default-time", value);
	}
} // namespace iridium::rclone::option
