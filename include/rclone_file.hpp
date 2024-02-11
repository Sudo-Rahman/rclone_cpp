#pragma once

#include <iostream>
#include <boost/date_time.hpp>
#include <rclone_remote.hpp>
#include <utility>

namespace Iridium
{
    class rclone_file
    {

    public:
        rclone_file(rclone_file *parent, const std::string &pathFile, uint64_t size, bool isDir,
                    boost::posix_time::ptime modTime,
                    const remote_ptr &remoteInfo);


        rclone_file(rclone_file *parent,const std::string &pathFile, uint64_t size, bool isDir, boost::posix_time::seconds modTime,
                    const remote_ptr &remoteInfo);

        rclone_file(const rclone_file &file) = default;

        rclone_file(rclone_file &&file) = default;

        rclone_file &operator=(const rclone_file &file) = default;

        rclone_file &operator=(rclone_file &&file) = default;

        ~rclone_file() = default;

        [[nodiscard]] std::string path() const { return _path; }

        [[nodiscard]] remote_ptr remote() const { return _remote; }

        [[nodiscard]] uint64_t size() const { return _size; }

        [[nodiscard]] rclone_file *parent() { return _parent; }

        [[nodiscard]] uint32_t nb_chilchren() const
        {
            if (_is_dir)
            {
                return _children.size();
            }
            return 0;
        }

        [[nodiscard]] bool is_dir() const { return _is_dir; }

        [[nodiscard]] boost::posix_time::ptime mod_time() const { return _mod_time; }

        void set_path(std::string path) { _path = std::move(path); }

        void set_remote(remote_ptr remote) { _remote = std::move(remote); }

        void set_size(uint64_t size) { _size = size; }

        void set_is_dir(bool isDir) { _is_dir = isDir; }

        void set_mod_time(boost::posix_time::ptime modTime) { _mod_time = modTime; }

        void se_mod_time(boost::posix_time::seconds modTime)
        {
            _mod_time = boost::posix_time::from_time_t(modTime.total_seconds());
        }

        void add_child(std::shared_ptr<rclone_file> child) { _children.push_back(std::move(child)); }

        void set_parent(rclone_file *parent) { _parent = parent; }

        [[nodiscard]] rclone_file *parent() const { return _parent; }

        [[nodiscard]] std::vector<std::shared_ptr<rclone_file>> children() const { return _children; }

        friend std::ostream &operator<<(std::ostream &os, const rclone_file &file);

        bool operator==(const rclone_file &rhs) const;

        bool operator!=(const rclone_file &rhs) const;

        static std::shared_ptr<rclone_file> create_shared_ptr(rclone_file *parent, const std::string &pathFile, uint64_t size, bool isDir,
                                                             boost::posix_time::ptime modTime, const remote_ptr &remoteInfo);

        static std::unique_ptr<rclone_file> create_unique_ptr(rclone_file *parent, const std::string &pathFile, uint64_t size, bool isDir,
                                                             boost::posix_time::ptime modTime, const remote_ptr &remoteInfo);

    private:
        rclone_file() = default;

        std::string _path{};
        remote_ptr _remote{};
        uint64_t _size{};
        rclone_file *_parent{};
        std::vector<std::shared_ptr<rclone_file>> _children{};
        bool _is_dir{};
        boost::posix_time::ptime _mod_time{};
    };

    typedef std::shared_ptr<rclone_file> rclone_file_ptr;

}
