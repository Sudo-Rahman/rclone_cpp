#pragma once

#include "../entities/file.hpp"
#include "basic_parser.hpp"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/json.hpp>
#include <iostream>
#include <regex>

namespace iridium::rclone::parser
{
    class file_parser : public basic_parser<entitie::file>
    {
        entitie::file * _parent;

    public:
        explicit file_parser(entitie::file * parent,
                             std::function<void(entitie::file)> callback)
            : basic_parser(std::move(callback)), _parent(parent) {}

        void parse(const std::string& data) const override;
    };
} // namespace iridium::rclone
