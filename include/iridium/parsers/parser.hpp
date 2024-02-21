#pragma once


#include "../entities/entitie.hpp"

namespace iridium::rclone
{
    template<class Type, class BaseClass>
    concept CheckType = std::is_base_of<BaseClass, Type>::value;

    template<class T> requires CheckType<T, entitie>
    class parser
    {
        void (*_callback)(const T &);

    public:

        explicit parser(void (*callback)(const T &)) : _callback(callback)
        {}


        parser(const parser &) = default;

        parser(parser &&) = default;

        parser &operator=(const parser &) = default;

        parser &operator=(parser &&) = default;

        virtual ~parser() = default;

        void parse(const std::string &data)
        {
            _callback(std::move(T().parse(data)));
        }

    };

} // namespace Iridium::process