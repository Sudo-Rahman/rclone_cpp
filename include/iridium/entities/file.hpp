#pragma once

#include <iostream>
#include <chrono>
#include <vector>
#include "remote.hpp"

namespace iridium::rclone::entities
{
	class file : public entity
	{
	public:
		file(file * parent, const std::string& file_name, int64_t size, bool is_dir,
		     const std::chrono::system_clock::time_point& mod_time,
		     const remote_ptr& remote = nullptr);

		file(file * parent, const std::string& file_name,const remote_ptr& remote = nullptr);

		[[nodiscard]] auto name() const -> std::string { return _name; }

		[[nodiscard]] virtual auto absolute_path() const -> std::string;

		[[nodiscard]] virtual auto path() const -> std::string;

		[[nodiscard]] auto remote() const -> remote_ptr { return _remote; }

		[[nodiscard]] auto size() const -> int64_t { return _size; }

		[[nodiscard]] auto parent() -> file* { return _parent; }

		[[nodiscard]] virtual auto parent_dir() const-> std::string;

		[[nodiscard]] virtual auto parent_absolute_dir() const-> std::string;

		[[nodiscard]] auto nb_chilchren() const -> uint32_t
		{
			if (_is_dir) { return _children.size(); }
			return 0;
		}

		[[nodiscard]] auto is_dir() const -> bool { return _is_dir; }

		[[nodiscard]] auto mod_time() const -> std::chrono::system_clock::time_point{ return _mod_time; }

		[[nodiscard]] auto pointer() -> file* { return this; }

		void set_name(const std::string& name);

		void set_remote(const remote_ptr& remote) { _remote = remote; }

		void set_size(const int64_t& size) { _size = size; }

		void set_is_dir(const bool& isDir) { _is_dir = isDir; }

		void set_mod_time(const std::chrono::system_clock::time_point& mod_time) { _mod_time = mod_time; }

		void add_child(const std::shared_ptr<file>& child);

		void add_child_if_not_exist(const std::shared_ptr<file>& child);

		void set_parent(file * parent) { _parent = parent; }

		[[nodiscard]] auto parent() const -> file* { return _parent; }

		[[nodiscard]] auto children() const -> std::vector<std::shared_ptr<file>> { return _children; }

		friend auto operator<<(std::ostream& os, const file& file) -> std::ostream&;

		auto operator==(const file& rhs) const -> bool;

		auto operator!=(const file& rhs) const -> bool;

		static auto
		create_shared_ptr(file * parent, const std::string& name_file, int64_t size, bool is_dir,
		                 std::chrono::system_clock::time_point mod_time, const remote_ptr& remote) -> std::shared_ptr<file>;

		file() = default;

		~file() override = default;

		file(const file&) = default;
		auto operator=(const file&) -> file & = default;

		file(file&&) noexcept;
		auto operator=(file&&) noexcept -> file&;

	private:
		std::string _name;
		remote_ptr _remote;
		int64_t _size{};
		file * _parent{};
		std::vector<std::shared_ptr<file>> _children;
		bool _is_dir{};
		std::chrono::system_clock::time_point _mod_time;
	};

	using file_ptr = std::shared_ptr<file>;
	using file_uptr = std::unique_ptr<file>;

} // namespace iridium::rclone
