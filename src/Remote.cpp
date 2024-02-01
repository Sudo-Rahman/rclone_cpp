//
// Created by sr-71 on 01/02/2024.
//

#include "Remote.hpp"

#include <utility>

namespace Iridium
{
    Remote::Remote(std::string name, RemoteType type, std::string path) :
            _name(std::move(name)), _type(type), _path(std::move(path))
    {}

    std::ostream &operator<<(std::ostream &os, const Remote &remote)
    {
        os << "\t\tRemote: " << std::endl <<
        "------------------------------------------------" << std::endl <<
        "name: " << remote._name << std::endl <<
        "type: " << remote._type << std::endl <<
        "path: " << remote._path << std::endl <<
        "------------------------------------------------" << std::endl;
        return os;
    }

    bool Remote::operator==(const Remote &remote) const
    {
        return _name == remote._name &&
               _type == remote._type &&
               _path == remote._path;
    }

    bool Remote::operator!=(const Remote &remote) const
    {
        return !(remote == *this);
    }
} // Iridium