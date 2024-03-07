#include <version.hpp>
#include <iostream>

namespace iridium::rclone{
    auto operator<<(std::ostream &os, const struct entitie::version &version) -> std::ostream & {
        os<< version.version << std::endl;
        os<< version.os_version << std::endl;
        os<< version.os_kernel << std::endl;
        os<< version.os_type << std::endl;
        os<< version.os_arch << std::endl;
        os<< version.go_version << std::endl;
        os<< version.go_linking << std::endl;
        os<< version.go_tags << std::endl;
        return os;
    }
}