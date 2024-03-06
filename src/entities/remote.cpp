#include <remote.hpp>

#include <utility>
#include <boost/algorithm/string/split.hpp>


namespace iridium::rclone
{
	entitie::remote::remote(std::string name, remote_type type, std::string path) : _name(std::move(name)), _type(type)
	{
		set_path(std::move(path));
	}

	auto entitie::remote::name() const -> std::string
	{
		if (_name.ends_with(":"))
			return _name.substr(0, _name.size() - 1);
		return _name;
	}

	auto entitie::remote::root_path() const -> std::string
	{
		if (not _name.ends_with(":"))
			return _name + ":";
		return _name;
	}

	auto entitie::remote::full_path() const -> std::string { return name() + ":" + path(); }

	auto operator<<(std::ostream& os, const entitie::remote& remote) -> std::ostream&
	{
		os << "Remote: {" << std::endl <<
				"\tname: " << remote._name << "," << std::endl <<
				"\ttype: " << remote._type << "," << std::endl <<
				"\tfull_path: " << remote.full_path() << std::endl << "}";
		return os;
	}

	void entitie::remote::set_path(std::string path)
	{
		_path = std::move(path);
		if (_path.ends_with("/"))
			_path = _path.substr(0, _path.size() - 1);
	}

	auto entitie::remote::operator==(const remote& remote) const -> bool
	{
		return _name == remote._name &&
		       _type == remote._type &&
		       _path == remote._path;
	}

	auto entitie::remote::operator!=(const remote& remote) const -> bool { return !(remote == *this); }

	auto
	entitie::remote::create_shared_ptr(std::string name, remote_type type,
	                                   std::string path) -> std::shared_ptr<entitie::remote>
	{
		return std::make_shared<remote>(std::move(name), type, std::move(path));
	}

	auto
	entitie::remote::create_unique_ptr(std::string name, remote_type type,
	                                   std::string path) -> std::unique_ptr<entitie::remote>
	{
		return std::make_unique<remote>(std::move(name), type, std::move(path));
	}

	const std::map<std::string, entitie::remote::remote_type> entitie::remote::string_to_remote_type = {
					{"drive", remote_type::google_drive},
					{"sftp", remote_type::sftp},
					{"onedrive", remote_type::onedrive},
					{"dropbox", remote_type::dropbox},
					{"ftp", remote_type::ftp},
					{"mega", remote_type::mega},
					{"opendrive", remote_type::opendrive},
					{"pcloud", remote_type::pcloud},
					{"box", remote_type::box},
					{"smb", remote_type::smb},
			};
} // Iridium
