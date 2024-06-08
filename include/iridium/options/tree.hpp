#pragma once

#include "basic_option.hpp"

namespace iridium::rclone::option::tree
{
	/**
	 * @brief All files are listed (list . files too)
	 * @return tree
	 */
	static auto all() -> basic_opt_uptr { return std::make_unique<basic_option>("-a"); }

	/**
	 * @brief List directories only
	 * @return tree
	 */
	static auto dirs_only() -> basic_opt_uptr { return std::make_unique<basic_option>("-d"); }

	/**
	 * @brief List directories before files (-U disables)
	 * @return tree
	 */
	static auto dirsfirst() -> basic_opt_uptr { return std::make_unique<basic_option>("--dirsfirst"); }

	/**
	 * @brief Print the full path prefix for each file
	 * @return tree
	 */
	static auto full_path() -> basic_opt_uptr { return std::make_unique<basic_option>("--full-path"); }

	/**
	 * @brief Help for tree
	 * @return tree
	 */
	static auto help() -> basic_opt_uptr { return std::make_unique<basic_option>("-h"); }

	/**
	 * @brief Print the date of last modification
	 * @return tree
	 */
	static auto modtime() -> basic_opt_uptr { return std::make_unique<basic_option>("-D"); }

	/**
	 * @brief Don't print indentation lines
	 * @return tree
	 */
	static auto noindent() -> basic_opt_uptr { return std::make_unique<basic_option>("--noindent"); }

	/**
	 * @brief Turn off file/directory count at end of tree listing
	 * @return tree
	 */
	static auto noreport() -> basic_opt_uptr { return std::make_unique<basic_option>("--noreport"); }

	/**
	 * @brief Print the protections for each file
	 * @return tree
	 */
	static auto protections() -> basic_opt_uptr { return std::make_unique<basic_option>("-p"); }

	/**
	 * @brief Quote filenames with double quotes
	 * @return tree
	 */
	static auto quote() -> basic_opt_uptr { return std::make_unique<basic_option>("-Q"); }

	/**
	 * @brief Print the size in bytes of each file
	 * @return tree
	 */
	static auto size() -> basic_opt_uptr { return std::make_unique<basic_option>("-s"); }

	/**
	 * @brief Sort files by last status change time
	 * @return tree
	 */
	static auto sort_ctime() -> basic_opt_uptr { return std::make_unique<basic_option>("--sort-ctime"); }

	/**
	 * @brief Sort files by last modification time
	 * @return tree
	 */
	static auto sort_modtime() -> basic_opt_uptr { return std::make_unique<basic_option>("-t"); }

	/**
	 * @brief Reverse the order of the sort
	 * @return tree
	 */
	static auto sort_reverse() -> basic_opt_uptr { return std::make_unique<basic_option>("-r"); }

	/**
	 * @brief Leave files unsorted
	 * @return tree
	 */
	static auto unsorted() -> basic_opt_uptr { return std::make_unique<basic_option>("-U"); }

	/**
	 * @brief Sort files alphanumerically by version
	 * @return tree
	 */
	static auto version() -> basic_opt_uptr { return std::make_unique<basic_option>("--version"); }

	/**
	 * @brief Descend only level directories deep
	 * @return tree
	 */
	static auto level(int level) -> basic_opt_uptr
	{
		return std::make_unique<basic_option>("--level", std::to_string(level));
	}

	/**
	 * @brief Output to file instead of stdout
	 * @return tree
	 */
	static auto output(const std::string& value) -> basic_opt_uptr
	{
		return std::make_unique<basic_option>("--output", value);
	}

	/**
	 * @brief Select sort: name, version, size, mtime, ctime
	 * @return tree
	 */
	class sort : public basic_option
	{
	public:
		enum sort_by { name, version, size, mtime, ctime };

		static auto uptr(sort_by sort_by) -> std::unique_ptr<sort> { return std::make_unique<sort>(sort_by); }
		explicit sort(sort_by sort_by) : basic_option("--sort", to_string(sort_by)) {}

		auto copy_uptr() -> basic_opt_uptr override
		{
			return std::make_unique<sort>(*this);
		}

	private:
		auto to_string(sort_by sort_by) -> std::string
		{
			switch (sort_by)
			{
				case name:
					return "name";
				case version:
					return "version";
				case size:
					return "size";
				case mtime:
					return "mtime";
				case ctime:
					return "ctime";
			}
			return "name";
		}
	};
} // namespace iridium::rclone::option::tree
