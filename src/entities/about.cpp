#include "entities/about.hpp"
#include <regex>
#include <iostream>


namespace iridium::rclone
{
    uint64_t convert_to_bytes(const std::string &input)
    {
        std::regex size(R"(([0-9]+)(.[0-9]+)?)");
        std::regex unit(R"((B|KiB|MiB|GiB|TiB))");
        std::smatch match;
        if (std::regex_search(input, match, size))
        {
            auto value = std::stold(match[0]);
            if (std::regex_search(input, match, unit))
            {
                std::string unit = match[1];
                if (unit == "B")
                {
                    return value;
                } else if (unit == "KiB")
                {
                    return value * 1024;
                } else if (unit == "MiB")
                {
                    return value * 1024 * 1024;
                } else if (unit == "GiB")
                {
                    return value * 1024 * 1024 * 1024;
                } else if (unit == "TiB")
                {
                    return value * 1024 * 1024 * 1024 * 1024;
                }
            }
        }
        return 0;
    }

    about about::create(const std::string &data)
    {
        auto about = iridium::rclone::about();
        std::regex total = std::regex("Total:.*\n");
        std::regex used = std::regex("Used:.*\n");
        std::regex free = std::regex("Free:.*\n");
        std::regex trash = std::regex("Trash:.*\n");
        std::regex other = std::regex("Other:.*");
        std::smatch match;
        if (std::regex_search(data, match, total))
        {
            about.total_bytes = convert_to_bytes(match[0]);
        }
        if (std::regex_search(data, match, used))
        {
            about.used_bytes = convert_to_bytes(match[0]);
        }
        if (std::regex_search(data, match, free))
        {
            about.free_bytes = convert_to_bytes(match[0]);
        }
        if (std::regex_search(data, match, trash))
        {
            about.trash_size = convert_to_bytes(match[0]);
        }
        if (std::regex_search(data, match, other))
        {
            about.other = convert_to_bytes(match[0]);
        }
        return about;
    }

    std::ostream &operator<<(std::ostream &os, const about &about)
    {
        os << "About: {" << std::endl <<
           "\ttotal_bytes: " << about.total_bytes << "," << std::endl <<
           "\tused_bytes: " << about.used_bytes << "," << std::endl <<
           "\tfree_bytes: " << about.free_bytes << "," << std::endl <<
           "\ttrash_size: " << about.trash_size << "," << std::endl <<
           "\tother: " << about.other << std::endl << "}";
        return os;
    }
}
