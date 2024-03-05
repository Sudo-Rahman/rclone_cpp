#pragma once

#include "basic_option.hpp"

namespace iridium::rclone::option
{
	class tree : public basic_option
	{
		explicit tree(const std::string& option, const std::string& value)
			: basic_option(option, value) {}

		explicit tree(const std::string& option)
			: basic_option(option) {}

	public:

		/**
		 * @brief All files are listed (list . files too)
		 * @return tree
		 */
		static auto all() -> tree { return tree("-a"); }

		/**
		 * @brief List directories only
		 * @return tree
		 */
	static auto dirs_only() -> tree { return tree("-d"); }

		/**
		 * @brief List directories before files (-U disables)
		 * @return tree
		 */
	static auto dirsfirst() -> tree { return tree("--dirsfirst"); }

		/**
		 * @brief Print the full path prefix for each file
		 * @return tree
		 */
	static auto full_path() -> tree { return tree("--full-path"); }

		/**
		 * @brief Help for tree
		 * @return tree
		 */
	static auto help() -> tree { return tree("-h"); }

		/**
		 * @brief Print the date of last modification
		 * @return tree
		 */
	static auto modtime() -> tree { return tree("-D"); }

		/**
		 * @brief Don't print indentation lines
		 * @return tree
		 */
	static auto noindent() -> tree { return tree("--noindent"); }

		/**
		 * @brief Turn off file/directory count at end of tree listing
		 * @return tree
		 */
	static auto noreport() -> tree { return tree("--noreport"); }

		/**
		 * @brief Print the protections for each file
		 * @return tree
		 */
	static auto protections() -> tree { return tree("-p"); }

		/**
		 * @brief Quote filenames with double quotes
		 * @return tree
		 */
	static auto quote() -> tree { return tree("-Q"); }

		/**
		 * @brief Print the size in bytes of each file
		 * @return tree
		 */
	static auto size() -> tree { return tree("-s"); }

		/**
		 * @brief Sort files by last status change time
		 * @return tree
		 */
	static auto sort_ctime() -> tree { return tree("--sort-ctime"); }

		/**
		 * @brief Sort files by last modification time
		 * @return tree
		 */
	static auto sort_modtime() -> tree { return tree("-t"); }

		/**
		 * @brief Reverse the order of the sort
		 * @return tree
		 */
	static auto sort_reverse() -> tree { return tree("-r"); }

		/**
		 * @brief Leave files unsorted
		 * @return tree
		 */
	static auto unsorted() -> tree { return tree("-U"); }

		/**
		 * @brief Sort files alphanumerically by version
		 * @return tree
		 */
	static auto version() -> tree { return tree("--version"); }

		/**
		 * @brief Descend only level directories deep
		 * @return tree
		 */
	static auto level(const std::string& value) -> tree { return tree("--level", value); }

		/**
		 * @brief Output to file instead of stdout
		 * @return tree
		 */
	static auto output(const std::string& value) -> tree { return tree("--output", value); }

		/**
		 * @brief Select sort: name, version, size, mtime, ctime
		 * @return tree
		 */
	static auto sort(const std::string& value) -> tree { return tree("--sort", value); }

	};
} // namespace iridium::rclone::option
