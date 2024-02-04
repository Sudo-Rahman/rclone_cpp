//
// Created by sr-71 on 01/02/2024.
//

#include "rclone_file.hpp"
#include <iostream>
#include <boost/thread.hpp>

namespace Iridium
{
    rclone_file::rclone_file(rclone_file *parent, const std::string &pathFile, uint64_t size, bool isDir,
                             boost::posix_time::ptime modTime, const Iridium::remote_ptr &remoteInfo)
    {
        _parent = parent;
        _path = pathFile;
        _size = size;
        _is_dir = isDir;
        _mod_time = modTime;
        _remote = remoteInfo;
    }

    rclone_file::rclone_file(rclone_file *parent, const std::string &pathFile, uint64_t size, bool isDir,
                             boost::posix_time::seconds modTime, const Iridium::remote_ptr &remoteInfo)
    {
        _parent = parent;
        _path = pathFile;
        _size = size;
        _is_dir = isDir;
        _mod_time = boost::posix_time::from_time_t(modTime.total_seconds());
        _remote = remoteInfo;

    }

    std::ostream &operator<<(std::ostream &os, const Iridium::rclone_file &file)
    {
        os << "\t\tRcloneFile: " << std::endl <<
           "------------------------------------------------" << std::endl <<
           "path: " << file._path << std::endl <<
           "size: " << file._size << std::endl <<
           "isDir: " << file._is_dir << std::endl <<
           "modTime: " << file._mod_time << std::endl <<
           "rclone_remote: " << *file._remote << std::endl <<
           "------------------------------------------------" << std::endl;
        return os;
    }


    bool rclone_file::operator!=(const rclone_file &rhs) const
    {
//        return !(rhs == *this);
        return true;
    }

    bool rclone_file::operator==(const rclone_file &rhs) const
    {
        return _path == rhs._path &&
               _size == rhs._size &&
               _is_dir == rhs._is_dir &&
               _mod_time == rhs._mod_time &&
               _remote == rhs._remote;
    }

    std::shared_ptr<rclone_file>
    rclone_file::create_shared_ptr(rclone_file *parent, const std::string &pathFile, uint64_t size, bool isDir,
                                   boost::posix_time::ptime modTime, const remote_ptr &remoteInfo)
    {
        return std::make_shared<rclone_file>(parent, pathFile, size, isDir, modTime, remoteInfo);
    }

    std::unique_ptr<rclone_file>
    rclone_file::create_unique_ptr(rclone_file *parent, const std::string &pathFile, uint64_t size, bool isDir,
                                   boost::posix_time::ptime modTime, const remote_ptr &remoteInfo)
    {
        return std::make_unique<rclone_file>(parent, pathFile, size, isDir, modTime, remoteInfo);
    }
} // Iridium
