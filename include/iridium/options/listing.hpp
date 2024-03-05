#pragma once

#include "basic_option.hpp"

namespace iridium::rclone::option
{
	class listing : public basic_option
	{
		explicit listing(const std::string& option, const std::string& value)
			: basic_option(option, value) {}

		explicit listing(const std::string& option)
            : basic_option(option) {}

	public:
		/**
		 * @brief Use recursive list if available; uses more memory but fewer transactions
		 * @return basic_option
		 */
		static auto fast_list() -> listing { return listing("--fast-list"); }

		/**
		 * @brief Time to show if modtime is unknown for files and directories (default 2000-01-01T00:00:00Z)
		 * @return basic_option
		 */
		static auto default_time(const std::string& value) -> listing
		{
			return listing("--default-time", value);
		}
	};
} // namespace iridium::rclone::option