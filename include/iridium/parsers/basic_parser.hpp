#pragma once

#include "../entities/entity.hpp"
#include <functional>
#include <string>

namespace iridium::rclone::parser
{
	template<class Type, class BaseClass>
	concept CheckType = std::is_base_of_v<BaseClass, Type>;

	template<class T> requires CheckType<T, entity>
	class basic_parser
	{
		std::function<void(const T&)> _callback;

	protected:
		explicit basic_parser(std::function<void(const T&)> callback)
			: _callback(std::move(callback)) {}

		void callback(const T& data) const { _callback(std::move(data)); }

	public:
		virtual void parse(const std::string& data) const = 0;

		virtual ~basic_parser() = default;

        static auto create(basic_parser* parser) -> std::shared_ptr<basic_parser>
        {
            return std::move(std::shared_ptr<basic_parser>(parser));
        }
	};
} // namespace iridium::rclone
