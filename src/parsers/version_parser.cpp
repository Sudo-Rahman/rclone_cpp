#include <version_parser.hpp>
#include <sstream>

namespace iridium::rclone::parser
{
	void version_parser::parse(const std::string &data) const
	{
		entity::version version;
		std::istringstream iss(data);
		std::string line;
		while (std::getline(iss, line))
		{
			if (line.find("rclone v") != std::string::npos) { version.rclone_version = line; }
			else if (line.find("os/version") != std::string::npos) { version.os_version = line; }
			else if (line.find("os/kernel") != std::string::npos) { version.os_kernel = line; }
			else if (line.find("os/type") != std::string::npos) { version.os_type = line; }
			else if (line.find("os/arch") != std::string::npos) { version.os_arch = line; }
			else if (line.find("go/version") != std::string::npos) { version.go_version = line; }
			else if (line.find("go/linking") != std::string::npos) { version.go_linking = line; }
			else if (line.find("go/tags") != std::string::npos) { version.go_tags = line; }
		}
		callback(version);
	}
}
