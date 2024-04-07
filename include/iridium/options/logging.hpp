#pragma once

#include "basic_option.hpp"

namespace iridium::rclone::option::logging
{
	/**
	 * @brief Log everything to this file
	 * @param value The file path
	 * @return A `basic_option` object with the option set
	 */
	static auto log_file(const std::string& value) -> basic_opt_uptr
	{
		return std::make_unique<basic_option>("--log-file", value);
	}

	/**
	 * @brief Set the log format string (comma separated list of options)
	 * @param value The desired format string
	 * @return A `basic_option` object with the option set
	 */
	static auto log_format(const std::string& value) -> basic_opt_uptr
	{
		return std::make_unique<basic_option>("--log-format", value);
	}

	/**
	 * @brief Set the log level
	 * @param level The desired log level (DEBUG, INFO, NOTICE, ERROR)
	 * @return A `basic_option` object with the option set
	 */
	class log_level : public basic_option
	{
	public:
		enum level { DEBUG, INFO, NOTICE, ERROR };


		static auto uptr(level level) -> std::unique_ptr<log_level> { return std::make_unique<log_level>(level); }
		explicit log_level(level level) : basic_option("--log-level", to_string(level)) {}

	private:
		std::string to_string(level level)
		{
			switch (level)
			{
				case DEBUG:
					return "DEBUG";
				case INFO:
					return "INFO";
				case NOTICE:
					return "NOTICE";
				case ERROR:
					return "ERROR";
			}
			return "INFO";
		}
	};

	/**
	 * @brief Activate systemd integration for the logger
	 * @return A `basic_option` object with the option set
	 */
	static auto log_systemd() -> basic_opt_uptr { return std::make_unique<basic_option>("--log-systemd"); }

	/**
	 * @brief Set the maximum number of stats groups to keep in memory
	 * @param value The maximum number of groups
	 * @return A `basic_option` object with the option set
	 */
	static auto max_stats_groups(int value) -> basic_opt_uptr
	{
		return std::make_unique<basic_option>("--max-stats-groups", std::to_string(value));
	}

	/**
	 * @brief Show progress during transfer
	 * @return A `basic_option` object with the option set
	 */
	static auto progress() -> basic_opt_uptr { return std::make_unique<basic_option>("--progress"); }

	/**
	 * @brief Show progress on the terminal title (requires -P/--progress)
	 * @return A `basic_option` object with the option set
	 */
	static auto progress_terminal_title() -> basic_opt_uptr
	{
		return std::make_unique<basic_option>("--progress-terminal-title");
	}

	/**
	 * @brief Print as little stuff as possible
	 * @return A `basic_option` object with the option set
	 */
	static auto quiet() -> basic_opt_uptr { return std::make_unique<basic_option>("--quiet"); }

	/**
	 * @brief Interval between printing stats (e.g. 500ms, 60s, 5m)
	 * @param duration The interval duration
	 * @return A `basic_option` object with the option set
	 */
	static auto stats(const std::string& duration) -> basic_opt_uptr
	{
		return std::make_unique<basic_option>("--stats", duration);
	}

	/**
	 * @brief Max file name length in stats (0 for no limit)
	 * @param value The maximum length
	 * @return A `basic_option` object with the option set
	 */
	static auto stats_file_name_length(int value) -> basic_opt_uptr
	{
		return std::make_unique<basic_option>("--stats-file-name-length", std::to_string(value));
	}

	/**
	 * @brief Make the stats fit on one line
	 * @return A `basic_option` object with the option set
	 */
	static auto stats_one_line() -> basic_opt_uptr { return std::make_unique<basic_option>("--stats-one-line"); }

	/**
	 * @brief Enable --stats-one-line and add current date/time prefix
	 * @return A `basic_option` object with the option set
	 */
	static auto stats_one_line_date() -> basic_opt_uptr
	{
		return std::make_unique<basic_option>("--stats-one-line-date");
	}

	/**
	 * @brief Enable --stats-one-line-date and use custom formatted date: Enclose date string in double quotes ("), see https://golang.org/pkg/time/#Time.Format
	 * @param format The desired date format string
	 * @return A `basic_option` object with the option set
	 */
	static auto stats_one_line_date_format(const std::string& format) -> basic_opt_uptr
	{
		return std::make_unique<basic_option>("--stats-one-line-date-format", format);
	}

	/**
	 * @brief Show data rate in stats as either 'bits' or 'bytes' per second
	 * @param unit The desired unit ('bits' or 'bytes')
	 * @return A `basic_option` object with the option set
	 */
	static auto stats_unit(const std::string& unit) -> basic_opt_uptr
	{
		return std::make_unique<basic_option>("--stats-unit", unit);
	}

	/**
	 * @brief Use Syslog for logging
	 * @return A `basic_option` object with the option set
	 */
	static auto syslog() -> basic_opt_uptr { return std::make_unique<basic_option>("--syslog"); }

	/**
	 * @brief Facility for syslog, e.g. KERN,USER,...
	 * @param facility The desired facility
	 * @return A `basic_option` object with the option set
	 */
	static auto syslog_facility(const std::string& facility) -> basic_opt_uptr
	{
		return std::make_unique<basic_option>("--syslog-facility", facility);
	}

	/**
	 * @brief Use json log format
	 * @return A `basic_option` object with the option set
	 */
	static auto use_json_log() -> basic_opt_uptr { return std::make_unique<basic_option>("--use-json-log"); }

	/**
	 * @brief Print lots more stuff (repeat for more)
	 * @return A `basic_option` object with the option set
	 */
	static auto verbose() -> basic_opt_uptr { return std::make_unique<basic_option>("--verbose"); }
}; // namespace iridium::rclone::option::logging
