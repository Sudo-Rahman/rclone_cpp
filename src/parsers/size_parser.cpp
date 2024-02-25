#include <size_parser.hpp>


namespace iridium::rclone::parser
{
	void size_parser::parse(const std::string& data) const
	{
		auto size = entitie::size();
		std::regex total = std::regex(R"(([0-9]+))");
		std::regex size_regex = std::regex(R"(([0-9]+ Byte))");
		std::smatch match;
		auto counter = 0;
		if (std::regex_search(data, match, total))
		{
			size.total_objects = std::stoull(match[0]);
			counter++;
		}
		if (std::regex_search(data, match, size_regex))
		{
			size.total_size = std::stoull(match[0]);
			counter++;
		}
		if (counter == 2)
			callback(size);
		else throw std::runtime_error("Error parsing size");
	}
} // namespace iridium::rclone::parser
