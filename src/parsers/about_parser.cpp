#include <about_parser.hpp>
#include <regex>

namespace iridium::rclone::parser
{
	auto convert_to_bytes(const std::string& input) -> uint64_t
	{
		std::regex size(R"(([0-9]+)(.[0-9]+)?)");
		std::regex unit(R"((B|KiB|MiB|GiB|TiB))");
		std::smatch match;
		if (std::regex_search(input, match, size))
		{
			auto value = std::stold(match[0]);
			if (std::regex_search(input, match, unit))
			{
				std::string unit = match[1];
				if (unit == "B") { return value; }
				if (unit == "KiB") { return value * 1024; }
				if (unit == "MiB") { return value * 1024 * 1024; }
				if (unit == "GiB") { return value * 1024 * 1024 * 1024; }
				if (unit == "TiB") { return value * 1024 * 1024 * 1024 * 1024; }
			}
		}
		return 0;
	}


	void about_parser::parse(const std::string& data) const
	{
		auto about = entitie::about();
		std::regex total = std::regex("Total:.*\n");
		std::regex used = std::regex("Used:.*\n");
		std::regex free = std::regex("Free:.*\n");
		std::regex trash = std::regex("Trashed:.*\n");
		std::regex other = std::regex("Other:.*");
		std::smatch match;
		if (std::regex_search(data, match, total)) { about.total_bytes = convert_to_bytes(match[0]); }
		if (std::regex_search(data, match, used)) { about.used_bytes = convert_to_bytes(match[0]); }
		if (std::regex_search(data, match, free)) { about.free_bytes = convert_to_bytes(match[0]); }
		if (std::regex_search(data, match, trash)) { about.trash_size = convert_to_bytes(match[0]); }
		if (std::regex_search(data, match, other)) { about.other = convert_to_bytes(match[0]); }
		callback(about);
	}
}
