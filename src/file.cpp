#include "file.hpp"
#include <iostream>
#include <boost/thread.hpp>
#include <boost/json.hpp>
#include <regex>

namespace Iridium::rclone
{
    file::file(file *parent, const std::string &name_file, uint64_t size, bool is_dir,
               boost::posix_time::ptime mod_time, const rclone_remote_ptr &remote)
    {
        _parent = parent;
        _name = name_file;
        _size = size;
        _is_dir = is_dir;
        _mod_time = mod_time;
        _remote = remote;
    }

    file::file(file *parent, const std::string &name_file, uint64_t size, bool is_dir,
               boost::posix_time::seconds mod_time,
               const rclone_remote_ptr &remote)
    {
        _parent = parent;
        _name = name_file;
        _size = size;
        _is_dir = is_dir;
        _mod_time = boost::posix_time::from_time_t(mod_time.total_seconds());
        _remote = remote;

    }

    std::ostream &operator<<(std::ostream &os, const file &file)
    {
        auto remote = std::string();
        auto string_stream = std::stringstream();
        if (file._remote)
        {
            string_stream << *file._remote;
            remote.append("\n");
            for (auto line = std::string(); std::getline(string_stream, line);)
                remote.append("\t\t" + line + "\n");
        }

        os << "RcloneFile: {" << std::endl <<
           "\tname: " << file._name << "," << std::endl <<
           "\tabsolute_path: " << file.absolute_path() << "," << std::endl <<
           "\tsize: " << file._size << "," << std::endl <<
           "\tisDir: " << file._is_dir << "," << std::endl <<
           "\tmodTime: " << file._mod_time << "," << std::endl <<
           "\trclone_remote: {" << remote << "\t}" << std::endl << "}";
        return os;
    }


    bool file::operator!=(const file &rhs) const
    {
        return !(rhs == *this);
    }

    bool file::operator==(const file &rhs) const
    {
        return _name == rhs._name &&
               _size == rhs._size &&
               _is_dir == rhs._is_dir &&
               _mod_time == rhs._mod_time &&
               _remote == rhs._remote;
    }

    std::shared_ptr<file>
    file::create_shared_ptr(file *parent, const std::string &name_file, uint64_t size, bool is_dir,
                            boost::posix_time::ptime mod_time, const rclone_remote_ptr &remote)
    {
        return std::make_shared<file>(parent, name_file, size, is_dir, mod_time, remote);
    }


    std::string file::absolute_path() const
    {
        std::string abs_path;
        if (_parent)
            abs_path = (_parent->absolute_path() + "/" + _name);
        else abs_path = _remote->full_path() + "/" + _name;

//        remove double slashes or more and replace with single slash
        auto regex = std::regex(R"([/]{2,})");
        return std::regex_replace(abs_path, regex, "/");
    }

    std::string file::path() const
    {
        std::string path;
        if (_parent)
            path = _parent->path() + "/" + _name;
        else path = "/" + _name;

        auto regex = std::regex(R"([/]{2,})");
        return std::regex_replace(path, regex, "/");
    }

    std::unique_ptr<file> file::from_json(const std::string &json_str, file *parent)
    {
        if (parent == nullptr)
            return nullptr;
        auto regex = std::regex(R"(\{.*\})");
        std::smatch match;
        if (std::regex_search(json_str, match, regex))
        {
            try
            {
                auto json = boost::json::parse(match[0].str());
                if (json.is_object())
                {
                    if (
                            not(json.as_object().contains("Name") &&
                                json.as_object().contains("Size") &&
                                json.as_object().contains("IsDir") &&
                                json.as_object().contains("ModTime"))
                            )
                        return nullptr;
//                                    modtime 2022-04-30T11:20:02.000Z
                    auto *tif = new boost::posix_time::time_input_facet;
                    tif->set_iso_extended_format();
                    std::istringstream iss(json.at("ModTime").as_string().c_str());
                    iss.imbue(std::locale(std::locale::classic(), tif));
                    boost::posix_time::ptime abs_time;
                    iss >> abs_time;
                    auto f = file(parent, json.at("Path").as_string().c_str(),
                                  json.at("Size").as_int64(),
                                  json.at("IsDir").as_bool(),
                                  abs_time,
                                  parent->remote());
                    parent->add_child(std::make_shared<file>(f));
                    return std::move(std::make_unique<file>(f));
                }
            } catch (const std::exception &e)
            {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }
        return nullptr;
    }
} // Iridium
