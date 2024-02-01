//
// Created by sr-71 on 01/02/2024.
//

#ifndef RCLONE_REMOTE_HPP
#define RCLONE_REMOTE_HPP

#include <iostream>

namespace Iridium
{
    enum RemoteType {
        GoogleDrive, Sftp, OneDrive, Dropbox, Ftp, Mega,
        OpenDrive, Pcloud, Box, Smb, Cmd, None
    };

    class Remote {

    public:
        Remote() = default;

        Remote(std::string name, RemoteType type, std::string path);

        Remote(const Remote &remote) = default;

        Remote(Remote &&remote) = default;

        Remote &operator=(const Remote &remote) = default;

        Remote &operator=(Remote &&remote) = default;

        ~Remote() = default;

        [[nodiscard]] std::string name() const
        { return _name; }

        [[nodiscard]] RemoteType type() const
        { return _type; }

        [[nodiscard]] std::string path() const
        { return _path; }

        void setName(std::string name)
        { _name = std::move(name); }

        void setType(RemoteType type)
        { _type = type; }

        void setPath(std::string path)
        { _path = std::move(path); }

        bool operator==(const Remote &remote) const;

        bool operator!=(const Remote &remote) const;

        friend std::ostream &operator<<(std::ostream &os, const Remote &remote);

    private:
        std::string _name{};
        RemoteType _type{None};
        std::string _path{};
    };

} // Iridium

#endif //RCLONE_REMOTE_HPP
