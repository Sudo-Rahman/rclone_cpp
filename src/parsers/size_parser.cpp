#include <regex>
#include <size_parser.hpp>

using namespace iridium::rclone::entities;

namespace iridium::rclone::parser
{
	void size_parser::parse(const std::string &data) const
	{
		auto size = ::size();
		std::regex total = std::regex(R"(([0-9]+))");
		std::regex size_regex = std::regex(R"(([0-9]+ Byte))");
		std::smatch match;
		if (std::regex_search(data, match, total))
			size.total_objects = std::stoull(match[0]);
		if (std::regex_search(data, match, size_regex))
			size.total_size = std::stoull(match[0]);
		callback(size);
	}
} // namespace iridium::rclone::parser
