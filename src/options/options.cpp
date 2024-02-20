#include <options.hpp>

using namespace std;

namespace iridium::rclone
{


    option::tree::tree(const option::tree::flags &option)
    {
        _options.push_back(option::tree::to_string(option));
    }

    option::tree::tree(const option::tree::flags_value &option, const string &value)
    {
        _options.push_back(option::tree::to_string(option));
        _options.push_back(value);
    }

    std::string option::tree::to_string(option::tree::flags option)
    {
        switch (option)
        {
            case option::tree::all:
                return "-a";
            case option::tree::dirs_only:
                return "-d";
            case option::tree::dirsfirst:
                return "--dirsfirst";
            case option::tree::full_path:
                return "--full-path";
            case option::tree::help:
                return "-h";
            case option::tree::modtime:
                return "-D";
            case option::tree::noindent:
                return "--noindent";
            case option::tree::noreport:
                return "--noreport";
            case option::tree::protections:
                return "-p";
            case option::tree::quote:
                return "-Q";
            case option::tree::size:
                return "-s";
            case option::tree::sort_ctime:
                return "--sort-ctime";
            case option::tree::sort_modtime:
                return "-t";
            case option::tree::sort_reverse:
                return "-r";
            case option::tree::unsorted:
                return "-U";
            case option::tree::version:
                return "--version";
        }
        return "";
    }

    std::string option::tree::to_string(option::tree::flags_value option)
    {
        switch (option)
        {
            case level:
                return "--level";
            case output:
                return "--output";
            case sort:
                return "--sort";
        }
        return "";
    }

//    --------------------------------------------------------------------------------------------

    option::filter::filter(const option::filter::flags &option) : iridium::rclone::option()
    {
        _options.push_back(option::filter::to_string(option));
    }

    option::filter::filter(const option::filter::flags_value &option, const string &value) : iridium::rclone::option()
    {
        _options.push_back(option::filter::to_string(option));
        _options.push_back(value);
    }

    std::string option::filter::to_string(option::filter::flags option)
    {
        switch (option)
        {
            case option::filter::delete_excluded:
                return "--delete-excluded";
            case option::filter::ignore_case:
                return "--ignore-case";
        }
        return "";
    }

    std::string option::filter::to_string(option::filter::flags_value option)
    {
        switch (option)
        {
            case option::filter::exclude:
                return "--exclude";
            case option::filter::exclude_from:
                return "--exclude-from";
            case option::filter::exclude_if_present:
                return "--exclude-if-present";
            case option::filter::files_from:
                return "--files-from";
            case option::filter::files_from_raw:
                return "--files-from-raw";
            case option::filter::filter_file:
                return "--filter";
            case option::filter::filter_from:
                return "--filter-from";
            case option::filter::include:
                return "--include";
            case option::filter::include_from:
                return "--include-from";
            case option::filter::max_age:
                return "--max-age";
            case option::filter::max_depth:
                return "--max-depth";
            case option::filter::max_size:
                return "--max-size";
            case option::filter::metadata_exclude:
                return "--metadata-exclude";
            case option::filter::metadata_exclude_from:
                return "--metadata-exclude-from";
            case option::filter::metadata_filter:
                return "--metadata-filter";
            case option::filter::metadata_filter_from:
                return "--metadata-filter-from";
            case option::filter::metadata_include:
                return "--metadata-include";
            case option::filter::metadata_include_from:
                return "--metadata-include-from";
            case option::filter::min_age:
                return "--min-age";
            case option::filter::min_size:
                return "--min-size";
        }
        return "";
    }

    void option::add_option_to_vector(const option &option, std::vector<std::string> &vector)
    {
        vector.insert(vector.end(), option._options.begin(), option._options.end());
    }

    void option::add_options_to_vector(const vector &options, std::vector<std::string> &vector)
    {
        for (auto &opt: options)
            add_option_to_vector(opt, vector);
    }

//    -----------------------------------------------

    option::performance::performance(const std::string &option, const std::string &value) : iridium::rclone::option()
    {
        _options.push_back(option);
        _options.push_back(value);
    }

    option::listing::listing(const string &option, const string &value)
    {
        _options.push_back(option);
        if(not value.empty())
            _options.push_back(value);
    }
}; // namespace iridium::rclone