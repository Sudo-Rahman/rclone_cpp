#pragma once

#include "../entities/file.hpp"
#include "basic_parser.hpp"

namespace iridium::rclone::parser
{
    class file_parser : public basic_parser<entity::file>
    {
    public:

        enum parser_type
        {
            json,
            lsl
        };

        explicit file_parser(entity::file * parent,
                             std::function<void(entity::file)> callback, parser_type type = json)
            : basic_parser(std::move(callback)), _parent(parent), _type(type) {}

        void parse(const std::string& data) const override;

    private:
        entity::file * _parent;
        parser_type _type;

        auto json_parse(const std::string& data) const -> void;

        auto lsl_parse(const std::string& data) const -> void;
    };
} // namespace iridium::rclone
