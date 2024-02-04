//
// Created by sr-71 on 01/02/2024.
//

#include "rclone_remote.hpp"

#include <utility>

namespace Iridium
{
    rclone_remote::rclone_remote(std::string name, remote_type type, std::string path) :
            _name(std::move(name)), _type(type), _path(std::move(path)) {}

    std::ostream &operator<<(std::ostream &os, const rclone_remote &remote)
    {
        os << "\t\tRemote: " << std::endl <<
           "------------------------------------------------" << std::endl <<
           "name: " << remote._name << std::endl <<
           "type: " << remote._type << std::endl <<
           "path: " << remote._path << std::endl <<
           "------------------------------------------------" << std::endl;
        return os;
    }

    bool rclone_remote::operator==(const rclone_remote &remote) const
    {
        return _name == remote._name &&
               _type == remote._type &&
               _path == remote._path;
    }

    bool rclone_remote::operator!=(const rclone_remote &remote) const
    {
        return !(remote == *this);
    }

    std::shared_ptr<rclone_remote> rclone_remote::create_shared_ptr(std::string name, remote_type type, std::string path)
    {
        return std::make_shared<rclone_remote>(std::move(name), type, std::move(path));
    }

    std::unique_ptr<rclone_remote> rclone_remote::create_unique_ptr(std::string name, remote_type type, std::string path)
    {
        return std::make_unique<rclone_remote>(std::move(name), type, std::move(path));
    }
} // Iridium