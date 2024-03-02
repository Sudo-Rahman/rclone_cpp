#pragma once

#include "entities/json_log.hpp"
#include "basic_parser.hpp"


namespace iridium::rclone::parser
{
    class json_log_parser : public basic_parser<entitie::json_log>
    {
    public:
        explicit json_log_parser(std::function<void(const entitie::json_log&)> callback)
            : basic_parser(std::move(callback)) {};

        auto parse(const std::string& data) const -> void override;

        [[nodiscard]] struct entitie::json_log::stats  parse_stats(const std::string& data) const;
    };
} // namespace iridium::rclone