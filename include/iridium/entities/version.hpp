#pragma once

#include "entitie.hpp"
#include <string>

namespace iridium::rclone
{
	struct entitie::version : public entitie
	{
        std::string version;
		std::string os_version;
		std::string os_kernel;
		std::string os_type;
		std::string os_arch;
		std::string go_version;
		std::string go_linking;
		std::string go_tags;

        friend auto operator<<(std::ostream &os, const struct version &version) -> std::ostream &;
	};
} // namespace iridium::rclone