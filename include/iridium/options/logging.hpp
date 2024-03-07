#pragma once

#include "basic_option.hpp"

namespace iridium::rclone::option
{
    class logging : public basic_option {
    public:
        logging(const std::string &option, const std::string &value) :
                basic_option(option, value) {}

        logging(const std::string &option) :
                basic_option(option) {}

        /**
         * @brief Log everything to this file
         * @param value The file path
         * @return A `logging` object with the option set
         */
        static auto log_file(const std::string &value) -> logging {
            return logging("--log-file", value);
        }

        /**
         * @brief Set the log format string (comma separated list of options)
         * @param value The desired format string
         * @return A `logging` object with the option set
         */
        static auto log_format(const std::string &value) -> logging {
            return logging("--log-format", value);
        }

        /**
         * @brief Set the log level
         * @param level The desired log level (DEBUG, INFO, NOTICE, ERROR)
         * @return A `logging` object with the option set
         */
        static auto log_level(const std::string &level) -> logging {
            return logging("--log-level", level);
        }

        /**
         * @brief Activate systemd integration for the logger
         * @return A `logging` object with the option set
         */
        static auto log_systemd() -> logging {
            return logging("--log-systemd"); // Empty value for boolean options
        }

        /**
         * @brief Set the maximum number of stats groups to keep in memory
         * @param value The maximum number of groups
         * @return A `logging` object with the option set
         */
        static auto max_stats_groups(int value) -> logging {
            std::string string_value = std::to_string(value);
            return logging("--max-stats-groups", string_value);
        }

        /**
         * @brief Show progress during transfer
         * @return A `logging` object with the option set
         */
        static auto progress() -> logging { return logging("--progress"); }

        /**
         * @brief Show progress on the terminal title (requires -P/--progress)
         * @return A `logging` object with the option set
         */
        static auto progress_terminal_title() -> logging { return logging("--progress-terminal-title"); }

        /**
         * @brief Print as little stuff as possible
         * @return A `logging` object with the option set
         */
        static auto quiet() -> logging { return logging("--quiet"); }

        /**
         * @brief Interval between printing stats (e.g. 500ms, 60s, 5m)
         * @param duration The interval duration
         * @return A `logging` object with the option set
         */
        static auto stats(const std::string &duration) -> logging {
            return logging("--stats", duration);
        }

        /**
         * @brief Max file name length in stats (0 for no limit)
         * @param value The maximum length
         * @return A `logging` object with the option set
         */
        static auto stats_file_name_length(int value) -> logging {
            return logging("--stats-file-name-length", std::to_string(value));
        }

        /**
         * @brief Make the stats fit on one line
         * @return A `logging` object with the option set
         */
        static auto stats_one_line() -> logging { return logging("--stats-one-line"); }

        /**
         * @brief Enable --stats-one-line and add current date/time prefix
         * @return A `logging` object with the option set
         */
        static auto stats_one_line_date() -> logging { return logging("--stats-one-line-date"); }

        /**
         * @brief Enable --stats-one-line-date and use custom formatted date: Enclose date string in double quotes ("), see https://golang.org/pkg/time/#Time.Format
         * @param format The desired date format string
         * @return A `logging` object with the option set
         */
        static auto stats_one_line_date_format(const std::string &format) -> logging
        {
            return logging("--stats-one-line-date-format", format);
        }

        /**
         * @brief Show data rate in stats as either 'bits' or 'bytes' per second
         * @param unit The desired unit ('bits' or 'bytes')
         * @return A `logging` object with the option set
         */
        static auto stats_unit(const std::string &unit) -> logging {
            return logging("--stats-unit", unit);
        }

        /**
         * @brief Use Syslog for logging
         * @return A `logging` object with the option set
         */
        static auto syslog() -> logging { return logging("--syslog"); }

        /**
         * @brief Facility for syslog, e.g. KERN,USER,...
         * @param facility The desired facility
         * @return A `logging` object with the option set
         */
        static auto syslog_facility(const std::string &facility) -> logging {
            return logging("--syslog-facility", facility);
        }

        /**
         * @brief Use json log format
         * @return A `logging` object with the option set
         */
        static auto use_json_log() -> logging { return logging("--use-json-log"); }

        /**
         * @brief Print lots more stuff (repeat for more)
         * @param count The number of times to repeat the verbosity flag
         * @return A `logging` object with the option set
         */
        static auto verbose() -> logging {
            return logging("--verbose");
        }
    };
}; // namespace iridium::rclone::option