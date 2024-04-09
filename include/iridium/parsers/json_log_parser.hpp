#pragma once

#include "../entities/json_log.hpp"
#include "basic_parser.hpp"
#include <boost/json.hpp>

namespace iridium::rclone::parser
{
	class json_log_parser : public basic_parser<entities::json_log>
	{
	public:
		explicit json_log_parser(std::function<void(const entities::json_log &)> callback)
			: basic_parser(std::move(callback)) {}

		static auto ptr(std::function<void(const entities::json_log &)> callback) -> std::shared_ptr<json_log_parser>
		{
			return std::make_shared<json_log_parser>(std::move(callback));
		}

		auto parse(const std::string &data) const -> void override;

	private:
		[[nodiscard]] static auto parse_stats(const boost::json::object &) -> entities::json_log::stats;

		[[nodiscard]] static auto parse_transferring(
			const boost::json::value *) -> std::vector<entities::json_log::stats::transfer>;

		[[nodiscard]] static auto parse_transfer(const boost::json::object &) -> entities::json_log::stats::transfer;
	};
} // namespace iridium::rclone
