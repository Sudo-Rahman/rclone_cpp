#include "entities/size.hpp"
#include <regex>
#include <iostream>

namespace iridium::rclone
{
    size size::create(const std::string &data)
    {
        auto size = iridium::rclone::size();
        std::regex total = std::regex("[0-9]+\n");
        std::regex size_regex = std::regex("([0-9]+ Byte)");
        std::smatch match;
        if (std::regex_search(data, match, total))
        {
            size.total_objects = std::stoull(match[0]);
        }
        if (std::regex_search(data, match, size_regex))
        {
            size.total_size = std::stoull(match[0]);
        }
        return size;
    }

    std::ostream &operator<<(std::ostream &os, const size &size)
    {
        os << "RcloneSize: {" << std::endl <<
           "\ttotal_objects: " << size.total_objects << "," << std::endl <<
           "\ttotal_size: " << size.total_size << std::endl << "}";
        return os;
    }


} // namespace iridium::rclone