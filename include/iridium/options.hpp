#pragma once

#include <string>
#include <vector>
#include <any>
#include <iostream>


namespace iridium::rclone
{
	class option
	{
	public:
		[[nodiscard]] virtual auto get() -> std::vector<std::string> { return _options; };

		class filter;

		class tree;

		class performance;

		class listing;

	protected:
		std::vector<std::string> _options;

		option() = default;

		option(const option&) = default;

		option(option&&) = default;

		auto operator=(const option&) -> option& = default;

		auto operator=(option&&) -> option& = default;

		friend class process;

		struct option_allocator : std::allocator<option>
		{
			template<class U, class... Args>
			void construct(U * p, Args&&... args) { ::new((void *)p) U(std::forward<Args>(args)...); }

			template<class U>
			struct rebind
			{
				using other = option_allocator;
			};
		};

		friend struct option_allocator;

		using vector = std::vector<option, option_allocator>;

		static void add_option_to_vector(const option& option, std::vector<std::string>& vector);

		static void add_options_to_vector(const vector& options, std::vector<std::string>& vector);
	};


	class option::tree : public option
	{
	public:
		enum flags
		{
			all, // All files are listed (list . files too)
			dirs_only, // List directories only
			dirsfirst, // List directories before files (-U disables)
			full_path, // Print the full path prefix for each file
			help, // Help for tree
			modtime, // Print the date of last modification
			noindent, // Don't print indentation lines
			noreport, // Turn off file/directory count at end of tree listing
			protections, // Print the protections for each file
			quote, // Quote filenames with double quotes
			size, // Print the size in bytes of each file
			sort_ctime, // Sort files by last status change time
			sort_modtime, // Sort files by last modification time
			sort_reverse, // Reverse the order of the sort
			unsorted, // Leave files unsorted
			version // Sort files alphanumerically by version
		};

		enum flags_value
		{
			level, // Descend only level directories deep
			output, // Output to file instead of stdout
			sort, // Select sort: name, version, size, mtime, ctime
		};

		explicit tree(const flags& option);

		tree(const flags_value& option, const std::string& value);

	private:
		static auto to_string(flags option) -> std::string;

		static auto to_string(flags_value option) -> std::string;
	};

	//    -----------------------------------------------------------------------------------------------

	class option::filter : public option
	{
	public:
		enum flags
		{
			delete_excluded, // Delete files on dest excluded from sync
			ignore_case, // Ignore case in filters (case insensitive)
		};

		enum flags_value
		{
			exclude, // Exclude files matching pattern
			exclude_from, // Read file exclude patterns from file (use - to read from stdin)
			exclude_if_present, // Exclude directories if filename is present
			files_from, // Read list of source-file names from file (use - to read from stdin)
			files_from_raw,
			// Read list of source-file names from file without any processing of lines (use - to read from stdin)
			filter_file, // Add a file filtering rule
			filter_from, // Read file filtering patterns from a file (use - to read from stdin)
			include, // Include files matching pattern
			include_from, // Read file include patterns from file (use - to read from stdin)
			max_age, // Only transfer files younger than this in s or suffix ms|s|m|h|d|w|M|y (default off)
			max_depth, // If set limits the recursion depth to this (default -1)
			max_size, // Only transfer files smaller than this in KiB or suffix B|K|M|G|T|P (default off)
			metadata_exclude, // Exclude metadatas matching pattern
			metadata_exclude_from, // Read metadata exclude patterns from file (use - to read from stdin)
			metadata_filter, // Add a metadata filtering rule
			metadata_filter_from, // Read metadata filtering patterns from a file (use - to read from stdin)
			metadata_include, // Include metadatas matching pattern
			metadata_include_from, // Read metadata include patterns from file (use - to read from stdin)
			min_age, // Only transfer files older than this in s or suffix ms|s|m|h|d|w|M|y (default off)
			min_size // Only transfer files bigger than this in KiB or suffix B|K|M|G|T|P (default off)
		};

		explicit filter(const flags& option);

		filter(const flags_value& option, const std::string& value);

	private:
		static auto to_string(flags option) -> std::string;

		static auto to_string(flags_value option) -> std::string;
	};

	class option::performance : public option
	{
		performance(const std::string& option, const std::string& value);

	public:
		///In memory buffer size when reading files for each --transfer (default 16Mi)
		static auto buffer_size(const std::string& value) -> performance
		{
			return option::performance("--buffer-size", value);
		};

		///Number of checkers to run in parallel (default 8)
		static auto checkers(int value) -> performance
		{
			return option::performance("--checkers", std::to_string(value));
		};

		///Number of file transfers to run in parallel (default 4)
		static auto transfers(int value) -> performance
		{
			return option::performance("--transfers", std::to_string(value));
		};
	};

	class option::listing : public option
	{
		explicit listing(const std::string& option, const std::string& value = "");

	public:
		///Use recursive list if available; uses more memory but fewer transactions
		static auto fast_list() -> listing { return option::listing("--fast-list"); }

		///Time to show if modtime is unknown for files and directories (default 2000-01-01T00:00:00Z)
		static auto default_time(const std::string& value) -> listing
		{
			return option::listing("--default-time", value);
		}
	};
}
