#include <json_log.hpp>


namespace iridium::rclone
{
    entitie::json_log::~json_log()
    {
        delete _stats;
    }

    struct entitie::json_log::stats entitie::json_log::create_stats()
    {
        return {};
    }

    auto entitie::json_log::string_to_level(const std::string &level) -> entitie::json_log::log_level
    {
        if (level == "info")
            return entitie::json_log::log_level::info;
        if (level == "warning")
            return entitie::json_log::log_level::warning;
        if (level == "error")
            return entitie::json_log::log_level::error;
        return entitie::json_log::log_level::info;
    }

    auto operator<<(std::ostream &os, const entitie::json_log &log) -> std::ostream &
    {
        auto stats = std::string{};
        auto buffer = std::stringstream{};
        if (log._stats != nullptr) {
            buffer << *log._stats;
            stats.append("\n");
            for (auto line = std::string(); std::getline(buffer, line);)
                stats.append("\t\t" + line + "\n");
        }

        os << "json_log" << std::endl <<
           "{" << std::endl <<
           "\tlevel : " << entitie::json_log::level_to_string(log._level) << ", " << std::endl <<
           "\tmessage : " << log.message() << ", " << std::endl <<
           "\tsource : " << log.source() << ", " << std::endl <<
           "\ttime : " << log.time() << ", " << std::endl <<
           "\tstats : {" << stats << "\t}" << std::endl <<
           "}";
        return os;
    }

    auto entitie::json_log::level_to_string(const entitie::json_log::log_level &level) -> std::string
    {
        switch (level) {
            case entitie::json_log::log_level::info:
                return "info";
            case entitie::json_log::log_level::warning:
                return "warning";
            case entitie::json_log::log_level::error:
                return "error";
        }
        return "info";
    }

    auto operator<<(std::ostream &os, const entitie::json_log::stats &stats) -> std::ostream &
    {
        os << "stats : {" << std::endl <<
           "\tbytes : " << stats.bytes << ", " << std::endl <<
           "\tchecks : " << stats.checks << ", " << std::endl <<
           "\tdeletes : " << stats.deletes << ", " << std::endl <<
           "\telapsed_time : " << stats.elapsed_time << ", " << std::endl <<
           "\terrors : " << stats.errors << ", " << std::endl <<
           "\tfatal_error : " << boost::lexical_cast<std::string>(stats.fatal_error) << std::endl <<
           "\tlast_error : " << stats.last_error << std::endl <<
           "\trenames : " << stats.renames << std::endl <<
           "\tretry_error : " << boost::lexical_cast<std::string>(stats.retry_error) << std::endl <<
           "\tspeed : " << stats.speed << std::endl <<
           "\ttotal_bytes : " << stats.total_bytes << std::endl <<
           "\ttotal_checks : " << stats.total_checks << std::endl <<
           "\ttotal_transfers : " << stats.total_transfers << std::endl <<
           "\ttransfer_time : " << stats.transfer_time << std::endl <<
           "\ttransfers : " << stats.transfers << std::endl <<
           "}";
        return os;
    }


} // namespace iridium::rclone