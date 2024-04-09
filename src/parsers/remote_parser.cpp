#include <remote_parser.hpp>

using namespace iridium::rclone::entities;

namespace iridium::rclone::parser
{
	void remote_parser::parse(const std::string &data) const
	{
		remote remote;
		std::regex re(R"((\w+):\s+(\w+))");
		std::smatch match;
		if (std::regex_search(data, match, re))
		{
			try
			{
				remote.set_name(match[1]);
				remote.set_type(remote::string_to_remote_type.at(match[2]));
				callback(remote);
			}
			catch (const std::exception &e) { std::cerr << "Error: " << e.what() << std::endl; }
		}
	}
} // namespace iridium::rclone::parser
