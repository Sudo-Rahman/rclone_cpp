#pragma once

#include <string>
#include <memory>
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

        static about create(const std::string &);

        about &parse(const std::string &) override;

        friend std::ostream &operator<<(std::ostream &os, const about &about);

    };

} // namespace Iridium::process