#pragma once

#include <string>
#include <memory>


namespace iridium::rclone
{
    class about
    {
    public:
        uint64_t total_bytes;
        uint64_t used_bytes;
        uint64_t free_bytes;
        uint64_t trash_size;
        uint64_t other;

        static about create(const std::string &);

        ~about() = default;

        friend std::ostream &operator<<(std::ostream &os, const about &about);

    private:

        about() = default;

        about(const about &) = default;

        about(about &&) = default;

        about &operator=(const about &) = default;

        about &operator=(about &&) = default;

        [[nodiscard]] static std::unique_ptr<about> create() {
            return std::unique_ptr<about>(new about());
        }

        friend class process;
    };

} // namespace Iridium::process