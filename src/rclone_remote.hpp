//
// Created by sr-71 on 01/02/2024.
//

#ifndef RCLONE_RCLONE_REMOTE_HPP
#define RCLONE_RCLONE_REMOTE_HPP

#include <iostream>
#include <memory>

namespace Iridium
{
    enum remote_type
    {
        GoogleDrive, Sftp, OneDrive, Dropbox, Ftp, Mega,
        OpenDrive, Pcloud, Box, Smb, Cmd, None
    };

    class rclone_remote
    {

    public:
        rclone_remote() = default;

        rclone_remote(std::string name, remote_type type, std::string path);

        rclone_remote(const rclone_remote &remote) = default;

        rclone_remote(rclone_remote &&remote) = default;

        rclone_remote &operator=(const rclone_remote &remote) = default;

        rclone_remote &operator=(rclone_remote &&remote) = default;

        ~rclone_remote() = default;

        [[nodiscard]] std::string name() const { return _name; }

        [[nodiscard]] remote_type type() const { return _type; }

        [[nodiscard]] std::string path() const { return _path; }

        void set_name(std::string name) { _name = std::move(name); }

        void set_type(remote_type type) { _type = type; }

        void set_path(std::string path) { _path = std::move(path); }

        bool operator==(const rclone_remote &remote) const;

        bool operator!=(const rclone_remote &remote) const;

        friend std::ostream &operator<<(std::ostream &os, const rclone_remote &remote);

        static std::shared_ptr<rclone_remote> create_shared_ptr(std::string name, remote_type type, std::string path);

        static std::unique_ptr<rclone_remote> create_unique_ptr(std::string name, remote_type type, std::string path);

    private:
        std::string _name{};
        remote_type _type{None};
        std::string _path{};
    };

    typedef std::shared_ptr<rclone_remote> remote_ptr;

} // Iridium

#endif //RCLONE_RCLONE_REMOTE_HPP
