#include <remote.hpp>

#include <utility>

namespace Iridium::rclone
{
    remote::remote(std::string name, remote_type type, std::string path) :
            _name(std::move(name)), _type(type) {
        set_path(std::move(path));
    }

    std::ostream &operator<<(std::ostream &os, const remote &remote)
    {
        os << "Remote: {" << std::endl <<
           "\tname: " << remote._name << "," << std::endl <<
           "\ttype: " << remote._type << "," << std::endl <<
           "\tpath: " << remote._path << std::endl << "}";
        return os;
    }

    bool remote::operator==(const remote &remote) const
    {
        return _name == remote._name &&
               _type == remote._type &&
               _path == remote._path;
    }

    bool remote::operator!=(const remote &remote) const
    {
        return !(remote == *this);
    }

    std::shared_ptr<remote> remote::create_shared_ptr(std::string name, remote_type type, std::string path)
    {
        return std::make_shared<remote>(std::move(name), type, std::move(path));
    }

    std::unique_ptr<remote> remote::create_unique_ptr(std::string name, remote_type type, std::string path)
    {
        return std::make_unique<remote>(std::move(name), type, std::move(path));
    }

    const std::map<std::string, remote::remote_type> remote::string_to_remote_type = {
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