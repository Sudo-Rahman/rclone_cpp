#include <version.hpp>
#include <iostream>

namespace iridium::rclone::entities
{
	auto operator<<(std::ostream &os, const version &version) -> std::ostream&
	{
		os << version.rclone_version << std::endl;
		os << version.os_version << std::endl;
		os << version.os_kernel << std::endl;
		os << version.os_type << std::endl;
		os << version.os_arch << std::endl;
		os << version.go_version << std::endl;
		os << version.go_linking << std::endl;
		os << version.go_tags << std::endl;
		return os;
	}
}
