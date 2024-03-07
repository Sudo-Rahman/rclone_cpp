#pragma once

#include "entity.hpp"
#include <iostream>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace iridium::rclone
{
    namespace parser
    {
        class json_log_parser;
    };

    class entity::json_log : public entity {

    public:

        enum class log_level : uint8_t {
            info,
            warning,
            error,
        };

        struct stats;

        json_log() = default;

        virtual ~json_log();

        [[nodiscard]] auto level() const -> log_level { return _level; }

        [[nodiscard]] auto message() const -> std::string { return _message; }

        [[nodiscard]] auto source() const -> std::string { return _source; }

        [[nodiscard]] auto time() const -> boost::posix_time::ptime { return _time; }

        [[nodiscard]] const struct json_log::stats *get_stats() const { return _stats; }

        static auto string_to_level(const std::string &level) -> log_level;

        static auto level_to_string(const log_level &level) -> std::string;

        friend auto operator<<(std::ostream &os, const json_log &log) -> std::ostream &;

        friend class parser::json_log_parser;

    protected:

        log_level _level{};
        std::string _message;
        std::string _source;
        boost::posix_time::ptime _time;
        struct stats *_stats{};

    private:
        [[nodiscard]] static struct json_log::stats create_stats();
    };

    struct entity::json_log::stats {
        uint64_t bytes{};
        uint64_t checks{};
        uint64_t deletes{};
        double_t elapsed_time{};
        uint64_t errors{};
        bool fatal_error{};
        std::string last_error;
        uint64_t renames{};
        bool retry_error{};
        double_t speed{};
        double_t total_bytes{};
        uint64_t total_checks{};
        uint64_t total_transfers{};
        double_t transfer_time{};
        uint64_t transfers{};

        friend auto operator<<(std::ostream &os, const stats &stats) -> std::ostream &;

        friend class json_log;

    private:
        stats() = default;
    };

} // namespace iridium::rclone::entity