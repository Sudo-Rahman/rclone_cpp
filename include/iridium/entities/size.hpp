#pragma once

#include <string>

namespace iridium::rclone
{
    class size
    {
    public:
        uint64_t total_objects;
        uint64_t total_size;

        static size create(const std::string &);

        ~size() = default;

        friend std::ostream &operator<<(std::ostream &os, const size &size);

    private:

        size() = default;

        size(const size &) = default;

        size(size &&) = default;

        size &operator=(const size &) = default;

        size &operator=(size &&) = default;

        friend class process;
    };
}