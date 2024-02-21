#pragma once

#include <string>
#include "entitie.hpp"

namespace iridium::rclone
{
    class entitie::size : public entitie
    {
    public:
        uint64_t total_objects{};
        uint64_t total_size{};

        static size create(const std::string &);

        size &parse(const std::string &);

        friend std::ostream &operator<<(std::ostream &os, const size &size);

    };
}