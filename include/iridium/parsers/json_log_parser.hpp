#pragma once

#include "../entities/json_log.hpp"
#include "basic_parser.hpp"

namespace iridium::rclone::parser
{
	class json_log_parser : public basic_parser<entities::json_log>
	{
	public:
		explicit json_log_parser(std::function<void(const entities::json_log &)> callback)
			: basic_parser(std::move(callback)) {}

		static auto ptr(std::function<void(const entities::json_log &)> callback) -> std::shared_ptr<basic_parser>
		{
			return std::make_shared<json_log_parser>(std::move(callback));
		}

		auto parse(const std::string &data) const -> void override;
	};
} // namespace iridium::rclone
