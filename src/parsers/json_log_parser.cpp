#include <json_log_parser.hpp>
#include <boost/json.hpp>
#include <regex>
#include "utils.cpp"

using namespace iridium::rclone::entities;

template<class T>
extern auto get_from_obj(const boost::json::object &obj, const std::string &key) -> T;

extern auto string_to_mode_time(const std::string &time) -> system_clock::time_point;

namespace iridium::rclone::parser
{
	auto parse_transfer(const boost::json::object &obj) -> json_log::stats::transfer
	{
		auto transfer = json_log::stats::transfer();
		transfer.bytes = get_from_obj<uint64_t>(obj, "bytes");
		transfer.eta = get_from_obj_optional<uint64_t>(obj, "eta");
		transfer.group = get_from_obj<std::string>(obj, "group");
		transfer.name = get_from_obj<std::string>(obj, "name");
		transfer.percentage = get_from_obj<uint64_t>(obj, "percentage");
		transfer.size = get_from_obj<uint64_t>(obj, "size");
		transfer.speed = get_from_obj<double_t>(obj, "speed");
		transfer.speed_avg = get_from_obj<double_t>(obj, "speedAvg");
		transfer.src_fs = get_from_obj_optional<std::string>(obj, "srcFs");
		transfer.dst_fs = get_from_obj_optional<std::string>(obj, "dstFs");

		return transfer;
	}

	auto parse_transferring(
		const boost::json::value *value) -> std::vector<json_log::stats::transfer>
	{
		if (value == nullptr)
			return {};
		if (value->is_null()) return {};
		auto array = value->as_array();
		auto transferring = std::vector<json_log::stats::transfer>();
		for (const auto &obj: array)
			transferring.push_back(parse_transfer(obj.as_object()));

		return transferring;
	}

	auto parse_stats(const boost::json::object &obj) -> json_log::stats
	{
		auto stats = json_log::stats();

		try
		{
			stats.bytes = get_from_obj<uint64_t>(obj, "bytes");
			stats.checks = get_from_obj<uint64_t>(obj, "checks");
			stats.deletes = get_from_obj<uint64_t>(obj, "deletes");
			stats.elapsed_time = get_from_obj<double_t>(obj, "elapsedTime");
			stats.errors = get_from_obj<uint64_t>(obj, "errors");
			stats.eta = get_from_obj_optional<uint64_t>(obj, "eta");
			stats.fatal_error = get_from_obj<bool>(obj, "fatalError");
			stats.last_error = get_from_obj<std::string>(obj, "lastError");
			stats.renames = get_from_obj<uint64_t>(obj, "renames");
			stats.retry_error = get_from_obj<bool>(obj, "retryError");
			stats.speed = get_from_obj<double_t>(obj, "speed");
			stats.total_bytes = get_from_obj<uint64_t>(obj, "totalBytes");
			stats.total_checks = get_from_obj<uint64_t>(obj, "totalChecks");
			stats.total_transfers = get_from_obj<uint64_t>(obj, "totalTransfers");
			stats.transfer_time = get_from_obj<double_t>(obj, "transferTime");
			stats.transferring = parse_transferring(obj.if_contains("transferring"));
			stats.transfers = get_from_obj<uint64_t>(obj, "transfers");
			stats.server_side_copies = get_from_obj_optional<uint64_t>(obj, "serverSideCopies");
			stats.server_side_copy_bytes = get_from_obj_optional<uint64_t>(obj, "serverSideCopyBytes");
			stats.server_side_move_bytes = get_from_obj_optional<uint64_t>(obj, "serverSideMoveBytes");
			stats.server_side_moves = get_from_obj_optional<uint64_t>(obj, "serverSideMoves");
		}
		catch (const std::exception &e) { std::cerr << "Error parsing stats: " << e.what() << " " << obj << std::endl; }
		return stats;
	}

	auto json_log_parser::parse(const std::string &data) const -> void
	{
		auto log = json_log();

		auto regex = std::regex(R"(\{.*\})");
		std::smatch match;
		if (!std::regex_search(data, match, regex))
		{
			std::cerr << "Error parsing JSON log, data does not match a json: " << data << std::endl;
			return;
		}

		try
		{
			boost::json::object obj = boost::json::parse(data).as_object();
			log._level = json_log::string_to_level(get_from_obj<std::string>(obj, "level"));
			log._message = get_from_obj<std::string>(obj, "msg");
			log._source = get_from_obj<std::string>(obj, "source");
			log._object = get_from_obj<std::string>(obj, "object");
			log._object_type = get_from_obj<std::string>(obj, "objectType");

			log._time = string_to_mode_time(get_from_obj<std::string>(obj, "time"));
			if (obj.contains("stats"))
				log._stats = new json_log::stats(parse_stats(obj.at("stats").as_object()));

			callback(log);
		}
		catch (const std::exception &e)
		{
			std::cerr << "Error parsing json log: " << e.what() << " " << data << std::endl;
		}
	}
} // namespace iridium::rclone::parser
