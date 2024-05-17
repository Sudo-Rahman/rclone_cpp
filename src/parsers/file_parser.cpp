#include <file_parser.hpp>
#include <chrono>
#include <boost/json.hpp>
#include <iostream>
#include <regex>

using std::chrono::system_clock;

extern auto string_to_mode_time(const std::string &time) -> system_clock::time_point;

using std::cout;
using std::endl;
using namespace iridium::rclone::entities;

namespace iridium::rclone::parser
{
	void file_parser::parse(const std::string &data) const
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

	auto dir_is_in_parent(const file *dir, const file *parent,
	                      bool (*prediact)(const file &, const file &) = nullptr) -> file *
	{
		file *find = nullptr;
		for (auto &child: parent->children())
		{
			if (prediact not_eq nullptr)
			{
				if (prediact(*child, *dir))
				{
					find = child.get();
					break;
				}
			}
			else if (*child == *dir)
			{
				find = child.get();
				break;
			}
		}
		return find;
	}

	auto file_parser::json_parse(const std::string &data) const -> void
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
					if (not(json.as_object().contains("Name") and
					        json.as_object().contains("Path") and
					        json.as_object().contains("Size") and
					        json.as_object().contains("IsDir") and
					        json.as_object().contains("ModTime"))) { return; }

					std::string path = json.at("Path").as_string().c_str();
					file *parent = _parent;

					while (path.find_first_of('/') not_eq std::string::npos)
					{
						auto file = std::make_shared<::file>(parent, path.substr(0, path.find_first_of('/')), -1,
						                                     true, string_to_mode_time(
							                                     json.at("ModTime").as_string().c_str()),
						                                     parent->remote());

						::file *dir = dir_is_in_parent(file.get(), parent,
						                               [](const ::file &f1, const ::file &f2)
						                               {
							                               return f1.name() == f2.name() and f1.is_dir() == f2.
							                                      is_dir();
						                               });
						if (dir not_eq nullptr)
							parent = dir;
						path = path.substr(path.find_first_of('/') + 1);
					}

					file file = ::file(parent, json.at("Name").as_string().c_str(),
					                   json.at("Size").as_int64(),
					                   json.at("IsDir").as_bool(),
					                   string_to_mode_time(json.at("ModTime").as_string().c_str()),
					                   parent->remote());
					parent->add_child(std::make_shared<::file>(file));
					callback(file);
				}
			}
			catch (const std::exception &e) { std::cerr << "Error: " << e.what() << std::endl; }
		}
	}

	auto file_parser::lsl_parse(const std::string &data) const -> void
	{
		// example out file :     26264 2024-02-06 15:09:45.212000000 Rahman_YILMAZ_M1_INFO.pdf
		auto regex = std::regex(R"((\d+)\s+(\d{4}-\d{2}-\d{2}\s+\d{2}:\d{2}:\d{2}\.\d{9})\s+(.*))");
		std::smatch match;
		if (std::regex_search(data, match, regex))
		{
			try
			{
				auto path = match[3].str();
				file *parent = _parent;

				while (path.find_first_of('/') not_eq std::string::npos)
				{
					auto file = std::make_shared<::file>(parent, path.substr(0, path.find_first_of('/')),
					                                     parent->remote());
					file->set_is_dir(true);
					file->set_size(-1);

					::file *dir = dir_is_in_parent(file.get(), parent);
					if (dir not_eq nullptr)
						parent = dir;
					else
					{
						parent->add_child(file);
						parent = file.get();
					}
					path = path.substr(path.find_first_of('/') + 1);
				}

				auto file = ::file(parent, path, std::stoull(match[1].str()),
				                   false, string_to_mode_time(match[2].str()),
				                   parent->remote());

				parent->add_child(std::make_shared<::file>(file));

				callback(file);
			}
			catch (const std::exception &e) { std::cerr << "Error: " << e.what() << std::endl; }
		}
	}
} // namespace iridium::rclone::parser
