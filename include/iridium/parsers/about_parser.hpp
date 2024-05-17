#pragma once

#include "basic_parser.hpp"
#include "../entities/about.hpp"

namespace iridium::rclone::parser
{
	class about_parser : public basic_parser<entities::about>
	{
	public:
		explicit about_parser(std::function<void(const entities::about &)> callback)
			: basic_parser(std::move(callback)) {}

		static auto ptr(std::function<void(const entities::about &)> callback) -> std::shared_ptr<about_parser>
		{
			return std::make_shared<about_parser>(std::move(callback));
		}

		void parse(const std::string &data) const override;
	};
} // namespace iridium::rclone::parser
