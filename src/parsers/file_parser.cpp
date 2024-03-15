#include <file_parser.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/json.hpp>
#include <iostream>
#include <regex>


namespace iridium::rclone::parser
{
	void file_parser::parse(const std::string& data) const
	{
		switch (_type)
		{
			case json:
				json_parse(data);
				break;
			case lsl:
				lsl_parse(data);
				break;
		}
	}

	auto file_parser::json_parse(const std::string& data) const -> void
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
					entity::file file = entity::file(_parent, json.at("Path").as_string().c_str(),
					                                 json.at("Size").as_int64(),
					                                 json.at("IsDir").as_bool(), abs_time,
					                                 _parent->remote());
					_parent->add_child(std::make_shared<entity::file>(file));
					callback(file);
				}
			}
			catch (const std::exception& e) { std::cerr << "Error: " << e.what() << std::endl; }
		}
	}

	auto file_parser::lsl_parse(const std::string& data) const -> void
	{
		// example out file :     26264 2024-02-06 15:09:45.212000000 Rahman_YILMAZ_M1_INFO.pdf
		auto regex = std::regex(R"((\d+)\s+(\d{4}-\d{2}-\d{2}\s+\d{2}:\d{2}:\d{2}\.\d{9})\s+(.*))");
		std::smatch match;
		if (std::regex_search(data, match, regex))
		{
			try
			{
				auto tif = new boost::posix_time::time_input_facet();
				tif->set_iso_extended_format();
				std::istringstream iss(match[2].str());
				iss.imbue(std::locale(std::locale::classic(), tif));

				boost::posix_time::ptime abs_time;
				iss >> abs_time;


				auto path = match[3].str();
				entity::file * parent = _parent;

				while (path.find_first_of('/') not_eq std::string::npos)
				{
					auto file = std::make_shared<entity::file>(parent, path.substr(0, path.find_first_of('/')),
					                                           parent->remote());
					file->set_is_dir(true);

					bool find = false;
					for (auto& child: parent->children())
					{
						if (*child == *file)
						{
							parent = child.get();
							find = true;
							break;
						}
					}
					if (not find)
					{
						parent->add_child(file);
						parent = file.get();
					}

					path = path.substr(path.find_first_of('/') + 1);
				}

				auto file = entity::file(parent, path, std::stoull(match[1].str()),
				                         false, abs_time,
				                         parent->remote());

				parent->add_child(std::make_shared<entity::file>(file));

				callback(file);
			}
			catch (const std::exception& e) { std::cerr << "Error: " << e.what() << std::endl; }
		}
	}
} // namespace iridium::rclone::parser
