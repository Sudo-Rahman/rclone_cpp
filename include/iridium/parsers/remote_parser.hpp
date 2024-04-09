#pragma once

#include "basic_parser.hpp"
#include "../entities/remote.hpp"
#include <regex>

namespace iridium::rclone::parser
{
	class remote_parser : public basic_parser<entities::remote>
	{
	public:
		explicit remote_parser(std::function<void(const entities::remote &)> callback)
			: basic_parser(std::move(callback)) {}

		static auto ptr(std::function<void(const entities::remote &)> callback) -> std::shared_ptr<remote_parser>
		{
			return std::make_shared<remote_parser>(std::move(callback));
		}

		void parse(const std::string &data) const override;
	};
} // namespace iridium::rclone
