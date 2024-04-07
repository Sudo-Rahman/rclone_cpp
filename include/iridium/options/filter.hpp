#pragma once

#include "basic_option.hpp"

namespace iridium::rclone::option::filter
{
	/**
	 * @brief Delete files on dest excluded from sync
	 * @return filter
	 */
	static auto delete_excluded() -> basic_opt_uptr { return std::make_unique<basic_option>("--delete-excluded"); }

	/**
	* @brief Ignore case in filters
	* @return filter
	*/
	static auto ignore_case() -> basic_opt_uptr { return std::make_unique<basic_option>("--ignore-case"); }

	/**
	 * @brief  Exclude files matching pattern
	 * @return filter
	 */
	static auto exclude(const std::string &value) -> basic_opt_uptr
	{
		return std::make_unique<basic_option>("--exclude", value);
	}

	/**
	 * @brief  Read file exclude patterns from file (use - to read from stdin)
	 * @return filter
	 */
	static auto exclude_from(const std::string &value) -> basic_opt_uptr
	{
		return std::make_unique<basic_option>("--exclude-from", value);
	}

	/**
	 * @brief Exclude directories if filename is present
	 * @return filter
	 */
	static auto exclude_if_present(const std::string &value) -> basic_opt_uptr
	{
		return std::make_unique<basic_option>("--exclude-if-present", value);
	}

	/**
	 * @brief Read list of source-file names from file (use - to read from stdin)
	 * @return filter
	 */
	static auto files_from(const std::string &value) -> basic_opt_uptr
	{
		return std::make_unique<basic_option>("--files-from", value);
	}

	/**
	 * @brief Read list of source-file names from file without any processing of lines (use - to read from stdin)
	 * @return filter
	 */
	static auto files_from_raw(const std::string &value) -> basic_opt_uptr
	{
		return std::make_unique<basic_option>("--files-from-raw", value);
	}

	/**
	 * @brief Add a file filtering rule
	 * @return filter
	 */
	class filter_file : public basic_option
	{
	public:
		[[nodiscard]] auto get() -> std::vector<std::string> override { return _files; }

		template<typename ...Args>
		static auto uptr(Args && ...args) -> std::unique_ptr<filter_file> requires (std::conjunction_v<
			std::is_convertible<Args, std::string> ...>)
		{
			return std::make_unique<filter_file>(std::forward<std::string>(args) ...);
		}

		template<typename ...Args>
		auto add_filter(Args && ...args) requires (std::conjunction_v<std::is_convertible<Args, std::string>
			...>)
		{
			for (const auto &arg: {std::forward<Args>(args) ...})
				_files.push_back("--filter=" + arg);
		}

		template<typename ...Args>
		explicit filter_file(Args && ...args) requires (std::conjunction_v<std::is_convertible<Args, std::string> ...>)
		{
			std::vector<std::string> arg_strings{std::forward<Args>(args) ...}; // Store arguments for file list
			for (const auto &arg: arg_strings)
				_files.push_back("--filter=" + arg);
			// Discard the first argument (pattern for main filter)
		}

	private:
		std::vector<std::string> _files;
	};

	/**
	 * @brief Read file filtering patterns from a file (use - to read from stdin)
	 * @return filter
	 */
	static auto filter_from(const std::string &value) -> basic_opt_uptr
	{
		return std::make_unique<basic_option>("--filter-from", value);
	}

	/**
	 * @brief  Include files matching pattern
	 * @return filter
	 */
	static auto include(const std::string &value) -> basic_opt_uptr
	{
		return std::make_unique<basic_option>("--include", value);
	}

	/**
	 * @brief Read file include patterns from file (use - to read from stdin)
	 * @return filter
	 */
	static auto include_from(const std::string &value) -> basic_opt_uptr
	{
		return std::make_unique<basic_option>("--include-from", value);
	}

	/**
	 * @brief Only transfer files younger than this in s or suffix ms|s|m|h|d|w|M|y (default off)
	 * @return filter
	 */
	static auto max_age(const std::string &value) -> basic_opt_uptr
	{
		return std::make_unique<basic_option>("--max-age", value);
	}

	/**
	 * @brief If set limits the recursion depth to this (default -1)
	 * @return filter
	 */
	static auto max_depth(int value) -> basic_opt_uptr
	{
		return std::make_unique<basic_option>("--max-depth", std::to_string(value));
	}

	/**
	* @brief Exclude files larger than this in the listing
	* @return filter
	*/
	static auto max_size(const std::string &value) -> basic_opt_uptr
	{
		return std::make_unique<basic_option>("--max-size", value);
	}

	/**
	 * @brief Exclude metadatas matching pattern
	 * @return filter
	 */
	static auto metadata_exclude(const std::string &value) -> basic_opt_uptr
	{
		return std::make_unique<basic_option>("--metadata-exclude", value);
	}

	/**
	 * @brief Read metadata exclude patterns from file (use - to read from stdin)
	 * @return filter
	 */
	static auto metadata_exclude_from(const std::string &value) -> basic_opt_uptr
	{
		return std::make_unique<basic_option>("--metadata-exclude-from", value);
	}

	/**
	 * @brief Add a metadata filtering rule
	 * @return filter
	 */
	static auto metadata_filter(const std::string &value) -> basic_opt_uptr
	{
		return std::make_unique<basic_option>("--metadata-filter", value);
	}

	/**
	 * @brief Read metadata filtering patterns from a file (use - to read from stdin)
	 * @return filter
	 */
	static auto metadata_filter_from(const std::string &value) -> basic_opt_uptr
	{
		return std::make_unique<basic_option>("--metadata-filter-from", value);
	}

	/**
	 * @brief Include metadatas matching pattern
	 * @return filter
	 */
	static auto metadata_include(const std::string &value) -> basic_opt_uptr
	{
		return std::make_unique<basic_option>("--metadata-include", value);
	}

	/**
	 * @brief Read metadata include patterns from file (use - to read from stdin)
	 * @return filter
	 */
	static auto metadata_include_from(const std::string &value) -> basic_opt_uptr
	{
		return std::make_unique<basic_option>("--metadata-include-from", value);
	}

	/**
	 * @brief Exclude files older than this in the listing
	 * @return filter
	 */
	static auto min_age(const std::string &value) -> basic_opt_uptr
	{
		return std::make_unique<basic_option>("--min-age", value);
	}

	/**
	 * @brief Exclude files smaller than this in the listing
	 * @return filter
	 */
	static auto min_size(const std::string &value) -> basic_opt_uptr
	{
		return std::make_unique<basic_option>("--min-size", value);
	}
};
