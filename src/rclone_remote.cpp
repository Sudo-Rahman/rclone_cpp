#include "rclone_remote.hpp"

#include <utility>

namespace Iridium
{
    rclone_remote::rclone_remote(std::string name, remote_type type, std::string path) :
            _name(std::move(name)), _type(type) {
        set_path(std::move(path));
    }

    std::ostream &operator<<(std::ostream &os, const rclone_remote &remote)
    {
        os << "Remote: {" << std::endl <<
           "\tname: " << remote._name << "," << std::endl <<
           "\ttype: " << remote._type << "," << std::endl <<
           "\tpath: " << remote._path << std::endl << "}";
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

    const std::map<std::string, rclone_remote::remote_type> rclone_remote::string_to_remote_type = {
            {"drive",     remote_type::google_drive},
            {"sftp",      remote_type::sftp},
            {"onedrive",  remote_type::onedrive},
            {"dropbox",   remote_type::dropbox},
            {"ftp",       remote_type::ftp},
            {"mega",      remote_type::mega},
            {"opendrive", remote_type::opendrive},
            {"pcloud",    remote_type::pcloud},
            {"box",       remote_type::box},
            {"smb",       remote_type::smb},
    };

} // Iridium