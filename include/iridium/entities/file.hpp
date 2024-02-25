#pragma once

#include <iostream>
#include <boost/date_time.hpp>
#include "remote.hpp"

namespace iridium::rclone
{
	class entitie::file : public entitie
	{
	public:
		file(file * parent, const std::string& name_file, uint64_t size, bool is_dir,
		     const boost::posix_time::ptime& mod_time,
		     const remote_ptr& remote);


		file(file * parent, const std::string& name_file, uint64_t size, bool is_dir,
		     const boost::posix_time::seconds& mod_time,
		     const remote_ptr& remote);

		[[nodiscard]] auto name() const -> std::string { return _name; }

		[[nodiscard]] auto absolute_path() const -> std::string;

		[[nodiscard]] auto path() const -> std::string;

		[[nodiscard]] auto remote() const -> remote_ptr { return _remote; }

		[[nodiscard]] auto size() const -> uint64_t { return _size; }

		[[nodiscard]] auto parent() -> file* { return _parent; }

		[[nodiscard]] auto nb_chilchren() const -> uint32_t
		{
			if (_is_dir) { return _children.size(); }
			return 0;
		}

		[[nodiscard]] auto is_dir() const -> bool { return _is_dir; }

		[[nodiscard]] auto mod_time() const -> boost::posix_time::ptime { return _mod_time; }

		[[nodiscard]] auto pointer() -> file* { return this; }

		void set_name(std::string path) { _name = std::move(path); }

		void set_remote(remote_ptr remote) { _remote = std::move(remote); }

		void set_size(uint64_t size) { _size = size; }

		void set_is_dir(bool isDir) { _is_dir = isDir; }

		void set_mod_time(boost::posix_time::ptime mod_time) { _mod_time = mod_time; }

		void se_mod_time(boost::posix_time::seconds mod_time)
		{
			_mod_time = boost::posix_time::from_time_t(mod_time.total_seconds());
		}

		void add_child(std::shared_ptr<file> child) { _children.push_back(std::move(child)); }

		void set_parent(file * parent) { _parent = parent; }

		[[nodiscard]] auto parent() const -> file* { return _parent; }

		[[nodiscard]] auto children() const -> std::vector<std::shared_ptr<file>> { return _children; }

		friend auto operator<<(std::ostream& os, const file& file) -> std::ostream&;

		auto operator==(const file& rhs) const -> bool;

		auto operator!=(const file& rhs) const -> bool;

		static auto
		create_shared_ptr(file * parent, const std::string& name_file, uint64_t size, bool is_dir,
		                  boost::posix_time::ptime mod_time, const remote_ptr& remote) -> std::shared_ptr<file>;

		file() = default;

	private:
		std::string _name;
		remote_ptr _remote;
		uint64_t _size{};
		file * _parent{};
		std::vector<std::shared_ptr<file>> _children;
		bool _is_dir{};
		boost::posix_time::ptime _mod_time;
	};
} // namespace iridium::rclone
