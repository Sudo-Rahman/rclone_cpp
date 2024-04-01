#pragma once

#include "../entities/json_log.hpp"
#include "basic_parser.hpp"
#include <boost/json.hpp>


namespace iridium::rclone::parser
{
    class json_log_parser : public basic_parser<entity::json_log>
    {
    public:
        explicit json_log_parser(std::function<void(const entity::json_log&)> callback)
            : basic_parser(std::move(callback)) {};

        auto parse(const std::string& data) const -> void override;

    private:
        [[nodiscard]] static auto parse_stats(const boost::json::object&) -> entity::json_log::stats ;

        [[nodiscard]] static auto parse_transferring(const boost::json::value*) -> std::vector<entity::json_log::stats::transfer>;

        [[nodiscard]] static auto parse_transfer(const boost::json::object&) -> entity::json_log::stats::transfer ;
    };
} // namespace iridium::rclone