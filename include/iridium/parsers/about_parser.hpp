#pragma once

#include <utility>

#include "basic_parser.hpp"
#include "../entities/about.hpp"

namespace iridium::rclone::parser
{
	class about_parser : public basic_parser<entitie::about>
	{
	public:
		explicit about_parser(std::function<void(const entitie::about&)> callback)
			: basic_parser(std::move(callback)) {}

		void parse(const std::string& data) const override;
	};
} // namespace iridium::rclone::parser
