#include <json_log.hpp>


namespace iridium::rclone
{
    entitie::json_log::~json_log()
    {
        delete _stats;
    }

    auto entitie::json_log::parse_stats(const std::string &data) -> struct stats
    {
        struct stats stats{};



        return stats;
    }

    struct entitie::json_log::stats entitie::json_log::create_stats() const
    {
    }


} // namespace iridium::rclone