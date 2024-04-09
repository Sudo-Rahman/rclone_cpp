#pragma once

#include "../entities/entity.hpp"
#include <functional>
#include <string>
#include <memory>

namespace iridium::rclone::parser
{
	template<class T> requires (std::is_base_of_v<entity, T>)
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
            return std::shared_ptr<basic_parser>(parser);
        }
	};

	using parser_ptr = std::shared_ptr<basic_parser<entity>>;
	using parser_uptr = std::unique_ptr<basic_parser<entity>>;
} // namespace iridium::rclone
