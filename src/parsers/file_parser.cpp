#include <file_parser.hpp>

namespace iridium::rclone::parser
{
	void file_parser::parse(const std::string& data) const
	{
		if (_parent == nullptr)
			return;
		auto regex = std::regex(R"(\{.*\})");
		std::smatch match;
		if (std::regex_search(data, match, regex))
		{
			try
			{
				auto json = boost::json::parse(match[0].str());
				if (json.is_object())
				{
					if (not(json.as_object().contains("Name") &&
					        json.as_object().contains("Size") &&
					        json.as_object().contains("IsDir") &&
					        json.as_object().contains("ModTime"))) { return; }
					//  modtime 2022-04-30T11:20:02.000Z
					auto tif = new boost::posix_time::time_input_facet();
					tif->set_iso_extended_format();
					std::istringstream iss(json.at("ModTime").as_string().c_str());
					iss.imbue(std::locale(std::locale::classic(), tif));

					boost::posix_time::ptime abs_time;
					iss >> abs_time;
					entitie::file file = entitie::file(_parent, json.at("Path").as_string().c_str(),
					                                   json.at("Size").as_int64(),
					                                   json.at("IsDir").as_bool(), abs_time,
					                                   _parent->remote());
					_parent->add_child(std::make_shared<entitie::file>(file));
					callback(file);
				}
			}
			catch (const std::exception& e) { std::cerr << "Error: " << e.what() << std::endl; }
		}
	}
} // namespace iridium::rclone::parser
