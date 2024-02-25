#include <file.hpp>
#include <iostream>
#include <boost/json.hpp>
#include <regex>

namespace iridium::rclone
{
	entitie::file::file(file * parent, const std::string& name_file, uint64_t size, bool is_dir,
	                    const boost::posix_time::ptime& mod_time, const remote_ptr& remote)
	{
		_parent = parent;
		_name = name_file;
		_size = size;
		_is_dir = is_dir;
		_mod_time = mod_time;
		_remote = remote;
	}

	entitie::file::file(file * parent, const std::string& name_file, uint64_t size, bool is_dir,
	                    const boost::posix_time::seconds& mod_time,
	                    const remote_ptr& remote)
	{
		_parent = parent;
		_name = name_file;
		_size = size;
		_is_dir = is_dir;
		_mod_time = boost::posix_time::from_time_t(mod_time.total_seconds());
		_remote = remote;
	}

	auto operator<<(std::ostream& os, const entitie::file& file) -> std::ostream&
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


	auto entitie::file::operator!=(const file& rhs) const -> bool { return !(rhs == *this); }

	auto entitie::file::operator==(const file& rhs) const -> bool
	{
		return _name == rhs._name &&
		       _size == rhs._size &&
		       _is_dir == rhs._is_dir &&
		       _mod_time == rhs._mod_time &&
		       _remote == rhs._remote;
	}

	auto
	entitie::file::create_shared_ptr(file * parent, const std::string& name_file, uint64_t size, bool is_dir,
	                                 boost::posix_time::ptime mod_time,
	                                 const remote_ptr& remote) -> std::shared_ptr<file>
	{
		return std::make_shared<file>(parent, name_file, size, is_dir, mod_time, remote);
	}


	auto entitie::file::absolute_path() const -> std::string
	{
		std::string abs_path;
		if (_parent)
			abs_path = (_parent->absolute_path() + "/" + _name);
		else abs_path = _remote->full_path() + "/" + _name;

		//        remove double slashes or more and replace with single slash
		auto regex = std::regex(R"([/]{2,})");
		return std::regex_replace(abs_path, regex, "/");
	}

	auto entitie::file::path() const -> std::string
	{
		std::string path;
		if (_parent)
			path = _parent->path() + "/" + _name;
		else path = "/" + _name;

		auto regex = std::regex(R"([/]{2,})");
		return std::regex_replace(path, regex, "/");
	}
} // Iridium::rclone
