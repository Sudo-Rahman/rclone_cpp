#include <file.hpp>
#include <iostream>
#include <boost/json.hpp>
#include <regex>

namespace iridium::rclone
{
	entity::file::file(file * parent, const std::string& file_name, int64_t size, bool is_dir,
	                   const boost::posix_time::ptime& mod_time, const remote_ptr& remote)
	{
		_parent = parent;
		set_name(file_name);
		_size = size;
		_is_dir = is_dir;
		_mod_time = mod_time;
		_remote = remote;
	}

	entity::file::file(file * parent, const std::string& file_name, int64_t size, bool is_dir,
	                   const boost::posix_time::seconds& mod_time,
	                   const remote_ptr& remote)
	{
		_parent = parent;
		set_name(file_name);
		_size = size;
		_is_dir = is_dir;
		_mod_time = boost::posix_time::from_time_t(mod_time.total_seconds());
		_remote = remote;
	}

	entity::file::file(file * parent, const std::string& file_name, const remote_ptr& remote)
	{
		_parent = parent;
		set_name(file_name);
		_remote = remote;
	}

	auto operator<<(std::ostream& os, const entity::file& file) -> std::ostream&
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


	auto entity::file::operator!=(const file& rhs) const -> bool { return !(rhs == *this); }

	void entity::file::add_child(const std::shared_ptr<file>& child) { _children.push_back(child); }

	void entity::file::add_child_if_not_exist(const std::shared_ptr<file>& child)
	{
		for (const auto& f: _children)
			if (*f == *child)
				return;
		add_child(child);
	}

	auto entity::file::operator==(const file& rhs) const -> bool
	{
		return _name == rhs._name &&
		       _size == rhs._size &&
		       _is_dir == rhs._is_dir &&
		       _mod_time == rhs._mod_time &&
		       _remote == rhs._remote;
	}

	auto
	entity::file::create_shared_ptr(file * parent, const std::string& name_file, int64_t size, bool is_dir,
	                                boost::posix_time::ptime mod_time,
	                                const remote_ptr& remote) -> std::shared_ptr<file>
	{
		return std::make_shared<file>(parent, name_file, size, is_dir, mod_time, remote);
	}

	entity::file::file(file&& file) noexcept
	{
		_parent = file._parent;
		file._parent = nullptr;
		_name = std::move(file._name);
		_size = file._size;
		file._size = 0;
		_is_dir = file._is_dir;
		file._is_dir = false;
		_mod_time = file._mod_time;
		file._mod_time = boost::posix_time::not_a_date_time;
		_remote = std::move(file._remote);
		_children = std::move(file._children);
	}

	auto entity::file::operator=(file&& f) noexcept -> file&
	{
		if (this == &f) return *this;
		_parent = f._parent;
		f._parent = nullptr;
		_name = std::move(f._name);
		_size = f._size;
		f._size = 0;
		_is_dir = f._is_dir;
		f._is_dir = false;
		_mod_time = f._mod_time;
		f._mod_time = boost::posix_time::not_a_date_time;
		_remote = std::move(f._remote);
		_children = std::move(f._children);

		return *this;
	}


	auto entity::file::absolute_path() const -> std::string
	{
		std::string abs_path;
		if (_parent)
			abs_path = _parent->absolute_path() + "/" + _name;
		else if (_remote) abs_path = _remote->full_path() + "/" + _name;
		else abs_path = _name;

		//        remove double slashes or more and replace with single slash
		auto regex = std::regex(R"([/]{2,})");
		return std::regex_replace(abs_path, regex, "/");
	}

	auto entity::file::path() const -> std::string
	{
		std::string path;
		if (_parent)
			path = _parent->path() + "/" + _name;
		else path = "/" + _name;

		auto regex = std::regex(R"([/]{2,})");
		return std::regex_replace(path, regex, "/");
	}

	auto entity::file::parent_dir()const -> std::string
	{
		if(_parent not_eq nullptr)
			return _parent->path();
		return "";
	}

	auto entity::file::parent_absolute_dir() const -> std::string
	{
		if(_parent not_eq nullptr)
			return _parent->absolute_path();
		return "";
	}


	void entity::file::set_name(const std::string& name)
	{
		auto regex = std::regex(R"([/]{2,})");
		_name = std::regex_replace(name, regex, "/");
	}
} // Iridium::rclone
