#pragma once

#include <iostream>
#include <boost/date_time.hpp>
#include "rclone_remote.hpp"
#include <utility>

namespace Iridium
{
    class rclone_file {

    public:
        rclone_file(rclone_file *parent, const std::string &name_file, uint64_t size, bool is_dir,
                    boost::posix_time::ptime mod_time,
                    const rclone_remote_ptr &remote);


        rclone_file(rclone_file *parent, const std::string &name_file, uint64_t size, bool is_dir,
                    boost::posix_time::seconds mod_time,
                    const rclone_remote_ptr &remote);

        rclone_file(const rclone_file &file) = default;

        rclone_file(rclone_file &&file) = default;

        rclone_file &operator=(const rclone_file &file) = default;

        rclone_file &operator=(rclone_file &&file) = default;

        ~rclone_file() = default;

        [[nodiscard]] std::string name() const
        { return _name; }

        [[nodiscard]] std::string absolute_path() const;

        [[nodiscard]] std::string path() const;

        [[nodiscard]] rclone_remote_ptr remote() const
        { return _remote; }

        [[nodiscard]] uint64_t size() const
        { return _size; }

        [[nodiscard]] rclone_file *parent()
        { return _parent; }

        [[nodiscard]] uint32_t nb_chilchren() const
        {
            if (_is_dir)
            {
                return _children.size();
            }
            return 0;
        }

        [[nodiscard]] bool is_dir() const
        { return _is_dir; }

        [[nodiscard]] boost::posix_time::ptime mod_time() const
        { return _mod_time; }

        [[nodiscard]] rclone_file *pointer()
        {
            return this;
        }

        void set_name(std::string path)
        { _name = std::move(path); }

        void set_remote(rclone_remote_ptr remote)
        { _remote = std::move(remote); }

        void set_size(uint64_t size)
        { _size = size; }

        void set_is_dir(bool isDir)
        { _is_dir = isDir; }

        void set_mod_time(boost::posix_time::ptime mod_time)
        { _mod_time = mod_time; }

        void se_mod_time(boost::posix_time::seconds mod_time)
        {
            _mod_time = boost::posix_time::from_time_t(mod_time.total_seconds());
        }

        void add_child(std::shared_ptr<rclone_file> child)
        { _children.push_back(std::move(child)); }

        void set_parent(rclone_file *parent)
        { _parent = parent; }

        [[nodiscard]] rclone_file *parent() const
        { return _parent; }

        [[nodiscard]] std::vector<std::shared_ptr<rclone_file>> children() const
        { return _children; }

        friend std::ostream &operator<<(std::ostream &os, const rclone_file &file);

        bool operator==(const rclone_file &rhs) const;

        bool operator!=(const rclone_file &rhs) const;

        static std::shared_ptr<rclone_file>
        create_shared_ptr(rclone_file *parent, const std::string &name_file, uint64_t size, bool is_dir,
                          boost::posix_time::ptime mod_time, const rclone_remote_ptr &remote);

        static std::unique_ptr<rclone_file> from_json(const std::string &json_str, rclone_file *parent);

    private:
        rclone_file() = default;

        std::string _name{};
        rclone_remote_ptr _remote{};
        uint64_t _size{};
        rclone_file *_parent{};
        std::vector<std::shared_ptr<rclone_file>> _children{};
        bool _is_dir{};
        boost::posix_time::ptime _mod_time{};

        friend class rclone;
    };

    typedef std::shared_ptr<rclone_file> rclone_file_ptr;

}
