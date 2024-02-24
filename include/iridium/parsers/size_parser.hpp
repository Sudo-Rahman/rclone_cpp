#pragma once

#include "parsers.hpp"
#include "../entities/size.hpp"

namespace iridium::rclone::parser
{
    class size_parser : public basic_parser<entitie::size> {
    public:
        explicit size_parser(std::function<void(const entitie::size &)> callback)
                : basic_parser(std::move(callback)) {};

        void  parse(const std::string &data)const override;
    };

}; // namespace iridium::rclone::parser
