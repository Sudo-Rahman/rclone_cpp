#pragma once

#include "basic_option.hpp"
#include <iomanip>
#include <iostream>

namespace iridium::rclone::option
{
	class filter : public basic_option
	{
		explicit filter(const std::string& option, const std::string& value)
			: basic_option(option, value) {}

		explicit filter(const std::string& option)
			: basic_option(option) {}

	public:
		/**
		 * @brief Delete files on dest excluded from sync
		 * @return filter
		 */
		static auto delete_excluded() -> filter { return filter("--delete-excluded", ""); }

		/**
		* @brief Ignore case in filters
		* @return filter
		*/
		static auto ignore_case() -> filter { return filter("--ignore-case"); }

		/**
		 * @brief  Exclude files matching pattern
		 * @return filter
		 */
		static auto exclude(const std::string& value) -> filter { return filter("--exclude", value); }

		/**
		 * @brief  Read file exclude patterns from file (use - to read from stdin)
		 * @return filter
		 */
		static auto exclude_from(const std::string& value) -> filter { return filter("--exclude-from", value); }

		/**
		 * @brief Exclude directories if filename is present
		 * @return filter
		 */
		static auto exclude_if_present(const std::string& value) -> filter
		{
			return filter("--exclude-if-present", value);
		}

		/**
		 * @brief Read list of source-file names from file (use - to read from stdin)
		 * @return filter
		 */
		static auto files_from(const std::string& value) -> filter { return filter("--files-from", value); }

		/**
		 * @brief Read list of source-file names from file without any processing of lines (use - to read from stdin)
		 * @return filter
		 */
		static auto files_from_raw(const std::string& value) -> filter { return filter("--files-from-raw", value); }

		/**
		 * @brief Add a file filtering rule
		 * @return filter
		 */
		static auto filter_file(const std::string& value) -> filter { return filter("--filter=" + value); }


		/**
		 * @brief Read file filtering patterns from a file (use - to read from stdin)
		 * @return filter
		 */
		static auto filter_from(const std::string& value) -> filter { return filter("--filter-from", value); }

		/**
		 * @brief  Include files matching pattern
		 * @return filter
		 */
		static auto include(const std::string& value) -> filter { return filter("--include", value); }

		/**
		 * @brief Read file include patterns from file (use - to read from stdin)
		 * @return filter
		 */
		static auto include_from(const std::string& value) -> filter { return filter("--include-from", value); }

		/**
		 * @brief Only transfer files younger than this in s or suffix ms|s|m|h|d|w|M|y (default off)
		 * @return filter
		 */
		static auto max_age(const std::string& value) -> filter { return filter("--max-age", value); }

		/**
		 * @brief If set limits the recursion depth to this (default -1)
		 * @return filter
		 */
		static auto max_depth(int value) -> filter { return filter("--max-depth", std::to_string(value)); }

		/**
		* @brief Exclude files larger than this in the listing
		* @return filter
		*/
		static auto max_size(const std::string& value) -> filter { return filter("--max-size", value); }


		/**
		 * @brief Exclude metadatas matching pattern
		 * @return filter
		 */
		static auto metadata_exclude(const std::string& value) -> filter { return filter("--metadata-exclude", value); }

		/**
		 * @brief Read metadata exclude patterns from file (use - to read from stdin)
		 * @return filter
		 */
		static auto metadata_exclude_from(const std::string& value) -> filter
		{
			return filter("--metadata-exclude-from", value);
		}

		/**
		 * @brief Add a metadata filtering rule
		 * @return filter
		 */
		static auto metadata_filter(const std::string& value) -> filter { return filter("--metadata-filter", value); }

		/**
		 * @brief Read metadata filtering patterns from a file (use - to read from stdin)
		 * @return filter
		 */
		static auto metadata_filter_from(const std::string& value) -> filter
		{
			return filter("--metadata-filter-from", value);
		}

		/**
		 * @brief Include metadatas matching pattern
		 * @return filter
		 */
		static auto metadata_include(const std::string& value) -> filter { return filter("--metadata-include", value); }

		/**
		 * @brief Read metadata include patterns from file (use - to read from stdin)
		 * @return filter
		 */
		static auto metadata_include_from(const std::string& value) -> filter
		{
			return filter("--metadata-include-from", value);
		}

		/**
		 * @brief Exclude files older than this in the listing
		 * @return filter
		 */
		static auto min_age(const std::string& value) -> filter { return filter("--min-age", value); }

		/**
		 * @brief Exclude files smaller than this in the listing
		 * @return filter
		 */
		static auto min_size(const std::string& value) -> filter { return filter("--min-size", value); }
	};
};
