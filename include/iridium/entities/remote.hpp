#pragma once

#include <iostream>
#include <memory>
#include <map>
#include <string>
#include "entitie.hpp"


namespace iridium::rclone
{
	class entitie::remote : public entitie
	{
	public:
		enum remote_type
		{
			google_drive, sftp, onedrive, dropbox, ftp, mega,
			opendrive, pcloud, box, smb, cmd, none
		};

		remote() = default;

		remote(std::string name, remote_type type, std::string path);

		remote(const remote& remote) = default;

		remote(remote&& remote) = default;

		auto operator=(const remote& remote) -> entitie::remote& = default;

		auto operator=(remote&& remote) -> entitie::remote& = default;

		~remote() = default;

		[[nodiscard]] auto name() const -> std::string
		{
			if (_name.ends_with(":"))
				return _name.substr(0, _name.size() - 1);
			return _name;
		}

		[[nodiscard]] auto root_path() const -> std::string
		{
			if (not _name.ends_with(":"))
				return _name + ":";
			return _name;
		}

		[[nodiscard]] auto type() const -> remote_type { return _type; }

		[[nodiscard]] auto path() const -> std::string { return _path; }

		[[nodiscard]] auto full_path() const -> std::string { return name() + ":" + path(); }


		void set_name(std::string name) { _name = std::move(name); }

		void set_type(remote_type type) { _type = type; }

		void set_path(std::string path)
		{
			_path = std::move(path);
			if (_path.ends_with("/"))
				_path = _path.substr(0, _path.size() - 1);
		}

		auto operator==(const remote& remote) const -> bool;

		auto operator!=(const remote& remote) const -> bool;

		friend auto operator<<(std::ostream& os, const remote& remote) -> std::ostream&;

		static auto create_shared_ptr(std::string name, remote_type type, std::string path) -> std::shared_ptr<remote>;

		static auto create_unique_ptr(std::string name, remote_type type, std::string path) -> std::unique_ptr<remote>;

	private:
		std::string _name;
		remote_type _type{none};
		std::string _path;

	public:
		static const std::map<std::string, remote_type> string_to_remote_type;
	};

	using remote_ptr = std::shared_ptr<entitie::remote>;
} // namespace iridium::rclone
