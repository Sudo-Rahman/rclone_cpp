#pragma once

#include <string>
#include <vector>
#include <any>


namespace Iridium::rclone
{
    class option
    {
    public:

        [[nodiscard]] virtual std::vector<std::string> get()
        { return _options; };

        class filter;

        class tree;

    protected:

        std::vector<std::string> _options;

        option() = default;

    private:
        option(const option &) = default;

        option(option &&) = default;

        option &operator=(const option &) = default;

        option &operator=(option &&) = default;

        static void add_options_to_vector(const std::vector <option> &options, std::vector <std::string> &vector);

        friend class process;

    };

    class option::tree : public option
    {
    public:
        enum flags
        {
            all,               // All files are listed (list . files too)
            dirs_only,         // List directories only
            dirsfirst,         // List directories before files (-U disables)
            full_path,         // Print the full path prefix for each file
            help,              // Help for tree
            modtime,           // Print the date of last modification
            noindent,          // Don't print indentation lines
            noreport,          // Turn off file/directory count at end of tree listing
            protections,       // Print the protections for each file
            quote,             // Quote filenames with double quotes
            size,              // Print the size in bytes of each file
            sort_ctime,        // Sort files by last status change time
            sort_modtime,      // Sort files by last modification time
            sort_reverse,      // Reverse the order of the sort
            unsorted,          // Leave files unsorted
            version            // Sort files alphanumerically by version
        };

        enum flags_value
        {
            level,             // Descend only level directories deep
            output,            // Output to file instead of stdout
            sort,              // Select sort: name, version, size, mtime, ctime
        };

        tree(const flags &option);

        tree(const flags_value &option, const std::string &value);

    private:
        static std::string to_string(flags option);

        static std::string to_string(flags_value option);
    };

//    -----------------------------------------------------------------------------------------------

    class option::filter : public option
    {
    public:
        enum flags
        {
            delete_excluded,                     // Delete files on dest excluded from sync
            ignore_case,                        // Ignore case in filters (case insensitive)
        };

        enum flags_value
        {
            exclude,                            // Exclude files matching pattern
            exclude_from,                       // Read file exclude patterns from file (use - to read from stdin)
            exclude_if_present,                 // Exclude directories if filename is present
            files_from,                         // Read list of source-file names from file (use - to read from stdin)
            files_from_raw,                     // Read list of source-file names from file without any processing of lines (use - to read from stdin)
            filter_file,                        // Add a file filtering rule
            filter_from,                        // Read file filtering patterns from a file (use - to read from stdin)
            include,                            // Include files matching pattern
            include_from,                       // Read file include patterns from file (use - to read from stdin)
            max_age,                            // Only transfer files younger than this in s or suffix ms|s|m|h|d|w|M|y (default off)
            max_depth,                          // If set limits the recursion depth to this (default -1)
            max_size,                           // Only transfer files smaller than this in KiB or suffix B|K|M|G|T|P (default off)
            metadata_exclude,                   // Exclude metadatas matching pattern
            metadata_exclude_from,              // Read metadata exclude patterns from file (use - to read from stdin)
            metadata_filter,                    // Add a metadata filtering rule
            metadata_filter_from,               // Read metadata filtering patterns from a file (use - to read from stdin)
            metadata_include,                   // Include metadatas matching pattern
            metadata_include_from,              // Read metadata include patterns from file (use - to read from stdin)
            min_age,                            // Only transfer files older than this in s or suffix ms|s|m|h|d|w|M|y (default off)
            min_size                            // Only transfer files bigger than this in KiB or suffix B|K|M|G|T|P (default off)
        };

        filter(const flags &option);

        filter(const flags_value &option, const std::string &value);

    private:
        static std::string to_string(flags option);

        static std::string to_string(flags_value option);
    };
}