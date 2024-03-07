#pragma once

#include "../entities/version.hpp"
#include "basic_parser.hpp"

namespace iridium::rclone::parser
{
    class version_parser : public basic_parser<entitie::version> {

    public :
        version_parser(std::function<void(entitie::version)> callback)
                : basic_parser(std::move(callback)) {}

        void parse(const std::string &data) const override;
    };

} // namespace iridium::rclone