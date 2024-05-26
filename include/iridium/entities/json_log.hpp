#pragma once

#include "entity.hpp"
#include <iostream>
#include <optional>
#include <chrono>
#include <cmath>
#include <vector>

namespace iridium::rclone { namespace parser
	{
		class json_log_parser;
	};

	namespace entities
	{
		class json_log : public entity
		{
		public:
			enum  log_level
			{
				no_level,
				info,
				warning,
				error,
			};

			struct stats;

			json_log() = default;

			json_log(const json_log &);

			auto operator=(const json_log &) -> json_log&;

			json_log(json_log &&) noexcept;

			auto operator=(json_log &&) noexcept -> json_log&;

			~json_log() override;

			[[nodiscard]] auto level() const -> log_level { return _level; }
			void set_level(log_level level) { _level = level; }

			[[nodiscard]] auto message() const -> std::string { return _message; }
			void set_message(const std::string &message) { _message = message; }

			[[nodiscard]] auto source() const -> std::string { return _source; }
			void set_source(const std::string &source) { _source = source; }

			[[nodiscard]] auto object() const -> std::string { return _object; }
			void set_object(const std::string &object) { _object = object; }

			[[nodiscard]] auto object_type() const -> std::string { return _object_type; }
			void set_object_type(const std::string &object_type) { _object_type = object_type; }

			[[nodiscard]] auto time() const -> std::chrono::system_clock::time_point { return _time; }
			void set_time(const std::chrono::system_clock::time_point &time) { _time = time; }

			[[nodiscard]] auto get_stats() const -> const stats * { return _stats; }
			void set_stats(std::unique_ptr<stats> stats) { _stats = stats.release(); }

			static auto string_to_level(const std::string &level) -> log_level;

			static auto level_to_string(const log_level &level) -> std::string;

			friend auto operator<<(std::ostream &os, const json_log &log) -> std::ostream&;

			friend class parser::json_log_parser;

		protected:
			log_level _level{};
			std::string _message;
			std::string _source;
			std::string _object;
			std::string _object_type;
			std::chrono::system_clock::time_point _time;
			stats *_stats{};
		};

		struct json_log::stats
		{
			struct transfer;

			uint64_t bytes{};
			uint64_t checks{};
			uint64_t deletes{};
			double_t elapsed_time{};
			uint64_t errors{};
			std::optional<uint64_t> eta;
			bool fatal_error{};
			std::string last_error;
			uint64_t renames{};
			bool retry_error{};
			double_t speed{};
			uint64_t total_bytes{};
			uint64_t total_checks{};
			uint64_t total_transfers{};
			double_t transfer_time{};
			std::vector<transfer> transferring;
			uint64_t transfers{};

			friend auto operator<<(std::ostream &os, const stats &stats) -> std::ostream&;
		};

		struct json_log::stats::transfer
		{
			uint64_t bytes{};
			std::optional<uint64_t> eta;
			std::string group;
			std::string name;
			uint64_t percentage{};
			uint64_t size{};
			double_t speed{};
			double_t speed_avg{};

			friend auto operator<<(std::ostream &os, const transfer &transfer) -> std::ostream&;
		};
	}} // namespace iridium::rclone::entity
