#pragma once

#include <string>
#include <boost/process.hpp>
#include <boost/asio.hpp>
#include <functional>
#include "../entities.hpp"
#include "../options.hpp"
#include "../parsers/basic_parser.hpp"
#include "../parsers.hpp"
#include <boost/signals2.hpp>

namespace iridium::rclone
{
	class config_create;

	class process
	{
	public:
		process();

		~process() noexcept;

		enum class state : uint8_t
		{
			not_launched,
			running,
			finished,
			stopped = finished,
			error = finished
		};

		/**
		 * @brief Initialize the rclone process
		 * @param path_rclone the path to the rclone executable or if empty the system path will be used
		 */
		static auto initialize(const std::string &path_rclone = "") -> void;

		auto wait_for_start() -> process&;

		auto wait_for_finish() -> process&;

		auto close_input_pipe() -> process&;

		auto close_output_pipe() -> process&;

		auto close_error_pipe() -> process&;

		auto execute() -> process&;

		auto get() -> process * { return this; };

		[[nodiscard]] auto exit_code() const -> int;

		[[nodiscard]] auto get_state() const -> state { return _state; }

		[[nodiscard]] auto is_running() const -> bool { return _state == state::running; }

		[[nodiscard]] auto get_output() const -> std::vector<std::string> { return _output; }

		[[nodiscard]] auto get_error() const -> std::vector<std::string> { return _error; }

		[[nodiscard]] auto options() -> std::vector<option::basic_opt_uptr>& { return _local_options; }

		[[nodiscard]] static auto global_options() -> std::vector<option::basic_opt_uptr>& { return _global_options; }

		[[nodiscard]] auto commands() const -> std::string;

		auto stop() -> void;

		static const std::string endl;

		auto operator<<(const std::string &input) -> process&;

		auto write_input(const std::string &input) const -> void;

		auto every_line(std::function<void(const std::string &)> &&callback) -> process&;

		template<class T>
		auto every_line_parser(std::shared_ptr<parser::basic_parser<T>> parser) -> process&
		{
			_signal_every_line->connect([this, parser = std::move(parser)](const std::string &line)
			{
				parser->parse(line);
			});
			return *this;
		}

		auto on_finish(std::function<void(int)> &&callback) -> process&;

		auto on_start(std::function<void()> &&callback) -> process&;

		template<class T>
		auto on_finish_parser(std::shared_ptr<parser::basic_parser<T>> parser) -> process&
		{
			_signal_finish->connect([this, parser = std::move(parser)](int)
			{
				parser->parse(boost::algorithm::join(_output, endl));
			});
			return *this;
		}

		auto on_finish_error(std::function<void()> &&callback) -> process&;

		auto version() -> process&;

		auto list_remotes(std::function<void(const std::vector<remote_ptr> &)> &&) -> process&;

		auto list_remotes() -> process&;

		auto delete_remote(const entity::remote &remote) -> process&;

		auto config() -> process&;

		auto config_create() -> config_create;

		friend class config_create;

		auto lsjson(const entity::remote &remote) -> process&;

		auto lsjson(const entity::file &file) -> process&;

		/**
		 * \brief Lists the objects in the source path to standard output in a human readable format with size and path. Recurses by default.
		 * \param file
		 * \return
		 */
		auto ls(const entity::file &file) -> process&;

		auto lsl(const entity::file &file) -> process&;

		auto lsd(const entity::file &file) -> process&;

		auto lsf(const entity::file &file) -> process&;

		auto copy_to(const entity::file &source, const entity::file &destination) -> process&;

		auto move_to(const entity::file &source, const entity::file &destination) -> process&;

		auto delete_file(const entity::file &file) -> process&;

		auto mkdir(const entity::file &file) -> process&;

		auto cat(const entity::file &file) -> process&;

		auto about(const entity::remote &remote) -> process&;

		auto size(const entity::file &file) -> process&;

		auto tree(const entity::file &file) -> process&;

		auto bi_sync(const entity::file &source, const entity::file &destination) -> process&;

		auto clean_up(const entity::remote &remote) -> process&;

		auto copy_url(const std::string &url, const entity::file &destination) -> process&;

		/**
		 * @brief Compare the source and destination and check if the files are the same
		 */
		auto check(const entity::file &source, const entity::file &destination) -> process&;

		template<class ...Args>
		auto add_option(Args && ...args) -> process& requires(std::conjunction_v<std::is_convertible<Args,
			option::basic_opt_uptr> ...>)
		{
			(_local_options.push_back(std::forward<Args>(args)), ...);
			return *this;
		}

		template<class ...Args>
		static auto add_global_option(Args && ...args) -> void requires(std::conjunction_v<std::is_convertible<Args,
			option::basic_opt_uptr> ...>) { (_global_options.push_back(std::forward<Args>(args)), ...); }

		static void clear_global_options();

	private:
		static std::string _path_rclone;
		static bool _is_initialized;
		static std::vector<option::basic_opt_uptr> _global_options;

		std::mutex _mutex;
		std::condition_variable _cv;

		state _state{state::not_launched};

		boost::process::child _child;
		std::unique_ptr<boost::process::ipstream> _out;
		std::unique_ptr<boost::process::ipstream> _err;
		std::unique_ptr<boost::process::opstream> _in;

		/// 1 thread for wait child, 1 thread for reading output,
		/// 1 thread for reading error and 2 last threads computing the signals for every line and finish
		boost::asio::thread_pool _pool{5};
		std::atomic_uint8_t _counter_read{0};

		std::vector<std::string> _args;
		std::vector<std::string> _output;
		std::vector<std::string> _error;

		std::vector<option::basic_opt_uptr> _local_options;

		auto read_output() -> void;

		auto read_error() -> void;

		std::unique_ptr<boost::signals2::signal<void(const std::string &line)>> _signal_every_line;
		std::unique_ptr<boost::signals2::signal<void(int)>> _signal_finish;
		std::unique_ptr<boost::signals2::signal<void()>> _signal_start;
	};

	using process_ptr = std::shared_ptr<process>;
	using process_uptr = std::unique_ptr<process>;
} // namespace iridium::rclone
