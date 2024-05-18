#include <remote_parser.hpp>

using namespace iridium::rclone::entities;

using std::string;
using std::regex;
using std::smatch;
using std::endl;

namespace iridium::rclone::parser
{
	void remote_parser::parse(const std::string &data) const
	{
		remote remote;
		regex pattern(R"(([^:]+):\s+([^\s]+))");
		std::smatch match;

		if (std::regex_search(data, match, pattern)) {
			string type = match[2];

			remote.set_name(match[1]);
			remote.set_type(remote::string_to_remote_type.at( match[2]));

			callback(remote);
		} else {
			std::cerr << "Impossible to parse remote" << endl;
		}
	}
} // namespace iridium::rclone::parser
