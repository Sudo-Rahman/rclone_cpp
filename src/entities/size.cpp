#include "entities/size.hpp"
#include <iostream>

namespace iridium::rclone::entities
{
	auto operator<<(std::ostream &os, const size &size) -> std::ostream&
	{
		os << "RcloneSize: {" << std::endl <<
				"\ttotal_objects: " << size.total_objects << "," << std::endl <<
				"\ttotal_size: " << size.total_size << std::endl << "}";
		return os;
	}
} // namespace iridium::rclone
