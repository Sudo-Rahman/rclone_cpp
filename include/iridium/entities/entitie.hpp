#pragma once

namespace iridium::rclone
{
    class entitie
    {
    private:
        entitie() = default;

        friend class process;

    public:

        virtual entitie &parse(const std::string &json) = 0;

        class about;

        class size;

    };

}