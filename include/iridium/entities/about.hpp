#pragma once

#include <string>
#include "entitie.hpp"


namespace iridium::rclone
{
    class entitie::about : public entitie
    {
    public:
        uint64_t total_bytes{};
        uint64_t used_bytes{};
        uint64_t free_bytes{};
        uint64_t trash_size{};
        uint64_t other{};

        friend auto operator<<(std::ostream &os, const about &about) -> std::ostream &;
    };

} // namespace Iridium::process