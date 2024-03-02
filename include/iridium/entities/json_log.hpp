#pragma once

#include "entitie.hpp"
#include <iostream>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace iridium::rclone
{
    class entitie::json_log : public entitie {

    public:

        enum class log_level {
            info,
            warning,
            error,
        };

        struct stats;

        json_log() = default;

        virtual ~json_log();


        [[nodiscard]] auto level() const -> log_level { return _level; }

        [[nodiscard]] auto message() const -> std::string { return _message; }

        [[nodiscard]] auto time() const -> boost::posix_time::ptime { return _time; }

        [[nodiscard]] const struct json_log::stats* stats() const { return _stats; }

        friend class json_log_parser;

    protected:

        log_level _level{};
        std::string _message;
        boost::posix_time::ptime _time;
        struct stats *_stats{};

    private:
        static auto parse_stats(const std::string &data) -> struct stats;
        [[nodiscard]] struct json_log::stats create_stats() const;
    };

    struct entitie::json_log::stats {
        uint64_t bytes;
        uint64_t checks;
        uint64_t deletes;
        double_t elapsed_time;
        uint64_t errors;
        bool fatal_error;
        uint64_t renames;
        bool retry_error;
        double_t speed;
        double_t transfer_time;
        uint64_t transfers;
    private:
        stats() = default;
        friend class json_log;
    };


} // namespace iridium::rclone::entitie