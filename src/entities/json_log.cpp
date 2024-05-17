#include <json_log.hpp>
#include <boost/lexical_cast.hpp>

using namespace std::chrono;
namespace iridium::rclone::entities
{
	json_log::json_log(const json_log &log)
		: entity(log),
		  _level(log._level),
		  _message(log._message),
		  _source(log._source),
		  _object(log._object),
		  _object_type(log._object_type),
		  _time(log._time)
	{
		if (log._stats not_eq nullptr)
			_stats = new stats(*log._stats);
	}

	auto json_log::operator=(const json_log &log) -> json_log&
	{
		if (this == &log)
			return *this;

		entity::operator=(log);
		_level = log._level;
		_message = log._message;
		_source = log._source;
		_object = log._object;
		_object_type = log._object_type;
		_time = log._time;
		if (log._stats not_eq nullptr)
			_stats = new stats(*log._stats);
		return *this;
	}

	json_log::json_log(json_log &&log) noexcept : entity(log)
	{

		_level = log._level;
		log._level = log_level::no_level;
		_message = std::move(log._message);
		_source = std::move(log._source);
		_object = std::move(log._object);
		_object_type = std::move(log._object_type);
		_time = log._time;
		log._time = system_clock::time_point();
		_stats = log._stats;
		log._stats = nullptr;
	}

	auto json_log::operator=(json_log &&log) noexcept -> json_log&
	{
		if (this == &log)
			return *this;

		entity::operator=(log);
		_level = log._level;
		log._level = log_level::no_level;
		_message = std::move(log._message);
		_source = std::move(log._source);
		_object = std::move(log._object);
		_object_type = std::move(log._object_type);
		_time = log._time;
		log._time = system_clock::time_point();
		_stats = log._stats;
		log._stats = nullptr;

		return *this;
	}

	json_log::~json_log() { delete _stats; }

	auto json_log::string_to_level(const std::string &level) -> log_level
	{
		if (level == "info")
			return log_level::info;
		if (level == "warning")
			return log_level::warning;
		if (level == "error")
			return log_level::error;
		return log_level::no_level;
	}

	auto operator<<(std::ostream &os, const json_log &log) -> std::ostream&
	{
		auto stats = std::string{};
		auto buffer = std::stringstream{};
		if (log._stats != nullptr)
		{
			buffer << *log._stats;
			stats.append("\n");
			for (auto line = std::string(); std::getline(buffer, line);)
				stats.append("\t\t" + line + "\n");
		}

		os << "json_log" << std::endl <<
				"{" << std::endl <<
				"\tlevel : " << json_log::level_to_string(log._level) << ", " << std::endl <<
				"\tmessage : " << log.message() << ", " << std::endl <<
				"\tsource : " << log.source() << ", " << std::endl <<
				"\tobject : " << log.object() << ", " << std::endl <<
				"\tobject_type : " << log.object_type() << ", " << std::endl <<
				"\ttime : " << log.time() << ", " << std::endl <<
				"\tstats : {" << stats << "\t}" << std::endl <<
				"}";
		return os;
	}

	auto json_log::level_to_string(const log_level &level) -> std::string
	{
		switch (level)
		{
			case log_level::info:
				return "info";
			case log_level::warning:
				return "warning";
			case log_level::error:
				return "error";
			default:
				return "no_level";
		}
	}

	auto operator<<(std::ostream &os, const json_log::stats &stats) -> std::ostream&
	{
		os << "stats : {" << std::endl <<
				"\tbytes : " << stats.bytes << ", " << std::endl <<
				"\tchecks : " << stats.checks << ", " << std::endl <<
				"\tdeletes : " << stats.deletes << ", " << std::endl <<
				"\telapsed_time : " << stats.elapsed_time << ", " << std::endl <<
				"\terrors : " << stats.errors << ", " << std::endl <<
				"\teta : " << (not stats.eta.has_value() ? "null" : std::to_string(stats.eta.value())) << ", " <<
				std::endl <<
				"\tfatal_error : " << boost::lexical_cast<std::string>(stats.fatal_error) << std::endl <<
				"\tlast_error : " << stats.last_error << std::endl <<
				"\trenames : " << stats.renames << std::endl <<
				"\tretry_error : " << boost::lexical_cast<std::string>(stats.retry_error) << std::endl <<
				"\tspeed : " << stats.speed << std::endl <<
				"\ttotal_bytes : " << stats.total_bytes << std::endl <<
				"\ttotal_checks : " << stats.total_checks << std::endl <<
				"\ttotal_transfers : " << stats.total_transfers << std::endl <<
				"\ttransfer_time : " << stats.transfer_time << std::endl <<
				"\transferring : [" << stats.transferring.size() << "]" << std::endl <<
				"\ttransfers : " << stats.transfers << std::endl <<
				"}";
		return os;
	}

	auto operator<<(std::ostream &os, const json_log::stats::transfer &transfer) -> std::ostream&
	{
		os << "transfer : {" << std::endl <<
				"\tbytes : " << transfer.bytes << ", " << std::endl <<
				"\teta : " << (not transfer.eta.has_value() ? "null" : std::to_string(transfer.eta.value())) << ", " <<
				std::endl <<
				"\tgroup : " << transfer.group << ", " << std::endl <<
				"\tname : " << transfer.name << std::endl <<
				"\tsize : " << transfer.size << std::endl <<
				"\tpercentage : " << transfer.percentage << std::endl <<
				"\tspeed : " << transfer.speed << std::endl <<
				"\tspeedAvg : " << transfer.speed_avg << std::endl <<
				"}";
		return os;
	}
} // namespace iridium::rclone
