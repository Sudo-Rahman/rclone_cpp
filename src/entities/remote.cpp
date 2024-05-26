#include <remote.hpp>

#include <utility>
#include <boost/algorithm/string/split.hpp>

namespace iridium::rclone::entities
{
	remote::remote(const std::string &name, remote_type type, const std::string &path) : _name(name),
		_type(type), _path(path)
	{
		if (type == none and not name.empty())
			throw std::runtime_error("Name be empty with type none");
	}

	remote::remote(remote &&remote) noexcept
	{
		_name = std::move(remote._name);
		_type = remote._type;
		remote._type = none;
		_path = std::move(remote._path);
	}

	auto remote::operator=(remote &&remote) noexcept -> entities::remote&
	{
		if (this == &remote) return *this;

		_name = std::move(remote._name);
		_type = remote._type;
		remote._type = none;
		_path = std::move(remote._path);

		return *this;
	}

	auto remote::name() const -> std::string
	{
		if (_name.ends_with(":"))
			return _name.substr(0, _name.size() - 1);
		return _name;
	}

	/**
	 * @return name and ":" if it doesn't end with ":"
	 */
	auto remote::root_path() const -> std::string
	{
		if (not _name.ends_with(":"))
		{
			if (_type == none or _name == "") return _name;
			return _name + ":";
		}
		return _name;
	}

	/**
	 * @return  root_path() + path()
	 */
	auto remote::full_path() const -> std::string { return root_path() + path(); }

	auto operator<<(std::ostream &os, const remote &remote) -> std::ostream&
	{
		os << "Remote: {" << std::endl <<
				"\tname: " << remote._name << "," << std::endl <<
				"\ttype: " << remote._type << "," << std::endl <<
				"\tfull_path: " << remote.full_path() << std::endl << "}";
		return os;
	}

	void remote::set_path(const std::string &path) { _path = path; }

	auto remote::operator==(const remote &remote) const -> bool
	{
		return _name == remote._name &&
		       _type == remote._type &&
		       _path == remote._path;
	}

	auto remote::operator!=(const remote &remote) const -> bool { return !(remote == *this); }

	auto
	remote::create_shared_ptr(const std::string &name, remote_type type,
	                          const std::string &path) -> std::shared_ptr<remote>
	{
		return std::make_shared<remote>(name, type, path);
	}

	auto
	remote::create_unique_ptr(const std::string &name, remote_type type,
	                          const std::string &path) -> std::unique_ptr<remote>
	{
		return std::make_unique<remote>(name, type, path);
	}

	auto remote::remote_type_to_string(const remote_type &type) -> const std::string
	{
		for (const auto &pair: string_to_remote_type)
		{
			if (pair.second == type)
				return pair.first;
		}
		return "none";
	}

	const std::map<std::string, remote::remote_type> remote::string_to_remote_type = {
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
