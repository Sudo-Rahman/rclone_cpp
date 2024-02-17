#pragma once

#include <string>
#include <vector>
#include <any>


namespace Iridium::rclone
{
    class option
    {
    public:

        [[nodiscard]] virtual std::vector<std::string> get() const = 0;

        class tree;

    protected:

        std::vector<std::string> _options;

        option() = default;

    private:
        option(const option &) = default;

        option(option &&) = default;

        option &operator=(const option &) = default;

        option &operator=(option &&) = default;

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

        [[nodiscard]] std::vector<std::string> get() const override
        { return _options; }

    private:
        static std::string to_string(flags option);

        static std::string to_string(flags_value option);
    };

}