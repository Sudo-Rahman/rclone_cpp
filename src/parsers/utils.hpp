#pragma once

#include <boost/json.hpp>
#include <boost/date_time.hpp>
#include <sstream>

using std::chrono::system_clock;

template<class T>
auto get_from_obj(const boost::json::object &obj, const std::string &key) -> T
{
	const auto *it = obj.if_contains(key);
	if (it)
		return boost::json::value_to<T>(obj.at(key));
	return T();
}

template<class T>
auto get_from_obj_optional(const boost::json::object &obj, const std::string &key) -> std::optional<T>
{
	const auto *it = obj.if_contains(key);
	if (it)
	{
		auto val = boost::json::try_value_to<T>(obj.at(key));
		if (val)
			return *val;
	}
	return std::nullopt;
}

auto string_to_mode_time(const std::string &time) -> system_clock::time_point
{
	auto tif = new boost::posix_time::time_input_facet();
	tif->set_iso_extended_format();
	std::istringstream iss(time);
	iss.imbue(std::locale(std::locale::classic(), tif));

	boost::posix_time::ptime abs_time;
	iss >> abs_time;

	boost::posix_time::ptime epoch(boost::gregorian::date(1970, 1, 1));
	boost::posix_time::time_duration diff = abs_time - epoch;

	return system_clock::from_time_t(diff.total_seconds());
}
