#include "entities/size.hpp"
#include <iostream>

namespace iridium::rclone
{

    std::ostream &operator<<(std::ostream &os, const entitie::size &size)
    {
        os << "RcloneSize: {" << std::endl <<
           "\ttotal_objects: " << size.total_objects << "," << std::endl <<
           "\ttotal_size: " << size.total_size << std::endl << "}";
        return os;
    }

} // namespace iridium::rclone