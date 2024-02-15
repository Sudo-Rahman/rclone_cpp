#pragma once

#include <string>
#include <memory>

namespace Iridium
{
    namespace rclone
    {
        class about {
        public:
            uint64_t total_bytes;
            uint64_t used_bytes;
            uint64_t free_bytes;
            uint64_t trash_size;
            uint64_t other;

            static std::unique_ptr<about> create(const std::string &);

        private:
            about() = default;
            about(const about&) = default;
            about(about&&) = default;
            about& operator=(const about&) = default;
            about& operator=(about&&) = default;
            ~about() = default;

            friend class rclone;
        };

    } // namespace rclone
}; // namespace Iridium