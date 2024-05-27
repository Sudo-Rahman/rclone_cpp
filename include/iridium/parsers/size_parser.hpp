#pragma once

#include "basic_parser.hpp"
#include "../entities/size.hpp"

namespace iridium::rclone::parser
{
	class size_parser : public basic_parser<entities::size>
	{
	public:
		explicit size_parser(std::function<void(const entities::size &)> callback)
			: basic_parser(std::move(callback)) {}

		static auto ptr(std::function<void(const entities::size &)> callback) -> std::shared_ptr<basic_parser>
		{
			return std::make_shared<size_parser>(std::move(callback));
		}

		void parse(const std::string &data) const override;
	};
}; // namespace iridium::rclone::parser
