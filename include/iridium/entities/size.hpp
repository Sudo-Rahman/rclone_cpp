#pragma once

#include <string>
#include "entity.hpp"

namespace iridium::rclone::entities
{
    class size : public entity
    {
    public:
        uint64_t total_objects{};
        uint64_t total_size{};

        friend auto operator<<(std::ostream &os, const size &size) -> std::ostream &;

    };
}