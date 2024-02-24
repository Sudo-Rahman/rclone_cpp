#pragma once

#include "basic_parser.hpp"
#include "../entities/remote.hpp"
#include <regex>

namespace iridium::rclone::parser
{
    class remote_parser : public basic_parser<entitie::remote> {
    public:
        explicit remote_parser(std::function<void(const entitie::remote &)> callback)
                : basic_parser(std::move(callback)) {};

        void  parse(const std::string &data)const override;

    };
} // namespace iridium::rclone