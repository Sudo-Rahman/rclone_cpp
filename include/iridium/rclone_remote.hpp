#pragma once

#include <iostream>
#include <memory>
#include <map>
#include <string>

namespace Iridium
{

    class rclone_remote {
    public:

        enum remote_type {
            google_drive, sftp, onedrive, dropbox, ftp, mega,
            opendrive, pcloud, box, smb, cmd, none
        };

        rclone_remote() = default;

        rclone_remote(std::string name, remote_type type, std::string path);

        rclone_remote(const rclone_remote &remote) = default;

        rclone_remote(rclone_remote &&remote) = default;

        rclone_remote &operator=(const rclone_remote &remote) = default;

        rclone_remote &operator=(rclone_remote &&remote) = default;

        ~rclone_remote() = default;

        [[nodiscard]] std::string name() const
        {
            if (_name.ends_with(":"))
                return _name.substr(0, _name.size() - 1);
            return _name;
        }

        [[nodiscard]] std::string root_path() const
        {
            if (not _name.ends_with(":"))
                return _name + ":";
            return _name;
        }

        [[nodiscard]] remote_type type() const
        { return _type; }

        [[nodiscard]] std::string path() const
        { return _path; }

        [[nodiscard]] std::string full_path() const
        {
            return name() + ":" + path();
        }


        void set_name(std::string name)
        { _name = std::move(name); }

        void set_type(remote_type type)
        { _type = type; }

        void set_path(std::string path)
        {
            _path = std::move(path);
            if(_path.ends_with("/"))
                _path = _path.substr(0, _path.size() - 1);
        }

        bool operator==(const rclone_remote &remote) const;

        bool operator!=(const rclone_remote &remote) const;

        friend std::ostream &operator<<(std::ostream &os, const rclone_remote &remote);

        static std::shared_ptr<rclone_remote> create_shared_ptr(std::string name, remote_type type, std::string path);

        static std::unique_ptr<rclone_remote> create_unique_ptr(std::string name, remote_type type, std::string path);

    private:
        std::string _name{};
        remote_type _type{none};
        std::string _path{};

    public:
        static const std::map<std::string, remote_type> string_to_remote_type;
    };

    typedef std::shared_ptr<rclone_remote> rclone_remote_ptr;

} // Iridium