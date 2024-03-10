#pragma once

#include "entity.hpp"
#include <string>

namespace iridium::rclone
{
	struct entity::version : entity
	{
        std::string rclone_version;
		std::string os_version;
		std::string os_kernel;
		std::string os_type;
		std::string os_arch;
		std::string go_version;
		std::string go_linking;
		std::string go_tags;

        friend auto operator<<(std::ostream &os, const version &version) -> std::ostream &;
	};
} // namespace iridium::rclone