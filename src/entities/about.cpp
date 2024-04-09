#include "entities/about.hpp"
#include <regex>
#include <iostream>

namespace iridium::rclone::entities
{
	auto operator<<(std::ostream &os, const about &about) -> std::ostream&
	{
		os << "About: {" << std::endl <<
				"\ttotal_bytes: " << about.total_bytes << "," << std::endl <<
				"\tused_bytes: " << about.used_bytes << "," << std::endl <<
				"\tfree_bytes: " << about.free_bytes << "," << std::endl <<
				"\ttrash_size: " << about.trash_size << "," << std::endl <<
				"\tother: " << about.other << std::endl << "}";
		return os;
	}
} // namespace iridium::rclone
