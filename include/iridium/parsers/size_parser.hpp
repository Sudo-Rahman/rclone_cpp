#pragma once

#include "parsers.hpp"
#include "../entities/size.hpp"

namespace iridium::rclone::parser
{
	class size_parser : public basic_parser<entity::size>
	{
	public:
		explicit size_parser(std::function<void(const entity::size&)> callback)
			: basic_parser(std::move(callback)) {};

		void parse(const std::string& data) const override;
	};
}; // namespace iridium::rclone::parser
