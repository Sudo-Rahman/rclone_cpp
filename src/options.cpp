#include <options.hpp>

using namespace std;

namespace Iridium::rclone
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
}