#pragma once

#include "basic_option.hpp"

namespace iridium::rclone::option
{
	class performance : public basic_option
	{
		performance(const std::string& option, const std::string& value) :
		basic_option(option, value)
		{};


	public:

		/**
		 * @brief In memory buffer size when reading files for each --transfer (default 16Mi)
		 * @return performance
		 */
		static auto buffer_size(const std::string& value) -> performance
		{
			return performance("--buffer-size", value);
		};

		/**
		 * @brief Number of checkers to run in parallel (default 8)
		 * @return performance
		 */
		static auto checkers(int value) -> performance
		{
			return performance("--checkers", std::to_string(std::move(value)));
		};

		/**
		 * @brief Number of file transfers to run in parallel (default 4)
		 * @return performance
		 */
		static auto transfers(int value) -> performance
		{
			return performance("--transfers", std::to_string(std::move(value)));
		};
	};
} // namespace iridium::rclone::option