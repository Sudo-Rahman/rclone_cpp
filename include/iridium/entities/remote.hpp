#pragma once

#include <iostream>
#include <memory>
#include <map>
#include <string>
#include "entity.hpp"


namespace iridium::rclone
{
	class entity::remote : public entity
	{
	public:
		enum remote_type
		{
			google_drive, sftp, onedrive, dropbox, ftp, mega,
			opendrive, pcloud, box, smb, cmd, none
		};

		remote() = default;

		remote(const std::string &name,remote_type type,const std::string& path);

		remote(const remote& remote) = default;

		remote(remote&& remote) = default;

		auto operator=(const remote& remote) -> entity::remote& = default;

		auto operator=(remote&& remote) -> entity::remote& = default;

		virtual ~remote() = default;

		[[nodiscard]] auto name() const -> std::string;

		[[nodiscard]] auto root_path() const -> std::string;

		[[nodiscard]] auto type() const -> remote_type { return _type; }

		[[nodiscard]] auto path() const -> std::string { return _path; }

		[[nodiscard]] auto full_path() const -> std::string;


		void set_name(std::string name) { _name = std::move(name); }

		void set_type(remote_type type) { _type = type; }

		void set_path(const std::string& path);

		auto operator==(const remote& remote) const -> bool;

		auto operator!=(const remote& remote) const -> bool;

		friend auto operator<<(std::ostream& os, const remote& remote) -> std::ostream&;

		static auto create_shared_ptr(const std::string &name,remote_type type,const std::string& path) -> std::shared_ptr<remote>;

		static auto create_unique_ptr(const std::string &name,remote_type type,const std::string& path) -> std::unique_ptr<remote>;

	private:
		std::string _name;
		remote_type _type{none};
		std::string _path;

	public:
		static const std::map<std::string, remote_type> string_to_remote_type;
		[[nodiscard]] static auto remote_type_to_string(const remote_type& type) -> const std::string;
	};

	using remote_ptr = std::shared_ptr<entity::remote>;
	using remote_uptr = std::unique_ptr<entity::remote>;
} // namespace iridium::rclone
