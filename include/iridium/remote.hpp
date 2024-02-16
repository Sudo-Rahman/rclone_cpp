#pragma once

#include <iostream>
#include <memory>
#include <map>
#include <string>

namespace Iridium::rclone
{

    class remote {
    public:

        enum remote_type {
            google_drive, sftp, onedrive, dropbox, ftp, mega,
            opendrive, pcloud, box, smb, cmd, none
        };

        remote() = default;

        remote(std::string name, remote_type type, std::string path);

        remote(const remote &remote) = default;

        remote(remote &&remote) = default;

        remote &operator=(const remote &remote) = default;

        remote &operator=(remote &&remote) = default;

        ~remote() = default;

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

        bool operator==(const remote &remote) const;

        bool operator!=(const remote &remote) const;

        friend std::ostream &operator<<(std::ostream &os, const remote &remote);

        static std::shared_ptr<remote> create_shared_ptr(std::string name, remote_type type, std::string path);

        static std::unique_ptr<remote> create_unique_ptr(std::string name, remote_type type, std::string path);

    private:
        std::string _name{};
        remote_type _type{none};
        std::string _path{};

    public:
        static const std::map<std::string, remote_type> string_to_remote_type;
    };

    typedef std::shared_ptr<remote> rclone_remote_ptr;

} // namespace Iridium::rclone