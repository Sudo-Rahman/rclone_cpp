#pragma once

#include <string>
#include <boost/process.hpp>
#include <boost/asio.hpp>
#include <functional>
#include "../entities.hpp"
#include "../options.hpp"
#include "../parsers/basic_parser.hpp"
#include "../parsers/file_parser.hpp"
#include <boost/signals2.hpp>

namespace iridium::rclone
{
	class process
	{
	public:
		process();

		~process();

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
		static auto initialize(const std::string& path_rclone = "") -> void;

		auto wait_for_start() -> process&;

		auto wait_for_finish() -> process&;

		auto close_input_pipe() -> process&;

		auto close_output_pipe() -> process&;

		auto close_error_pipe() -> process&;

		auto execute() -> process&;

		[[nodiscard]] auto exit_code() const -> int;

		[[nodiscard]] auto get_state() const -> state { return _state; }

        [[nodiscard]] auto is_running() -> bool { return _state == state::running;}

		[[nodiscard]] auto get_output() const -> std::vector<std::string> { return _output; }

		[[nodiscard]] auto get_error() const -> std::vector<std::string> { return _error; }

		[[nodiscard]] auto get_options() const -> option::vector { return _local_options; }

		[[nodiscard]] static auto get_global_options() -> option::vector { return _global_options; }

		auto stop() -> void;

		static const std::string endl;

		auto operator<<(const std::string& input) -> process&;

		auto write_input(const std::string& input) const -> void;

		auto every_line(std::function<void(const std::string&)>&& callback) -> process&;

		template<class T>
		auto every_line_parser(std::shared_ptr<parser::basic_parser<T>> parser) -> process&
		{
			_signal_every_line->connect([this, parser = std::move(parser)](const std::string& line) {parser->parse(line); });
			return *this;
		}

		auto on_finish(std::function<void(int)>&& callback) -> process&;

		template<class T>
		auto on_finish_parser(std::shared_ptr<parser::basic_parser<T>> parser) -> process&
		{
			_signal_finish->connect([this, parser = std::move(parser)](int) { parser.parse(boost::algorithm::join(_output, endl)); });
			return *this;
		}

		auto on_finish_error(std::function<void()>&& callback) -> process&;

		auto version() -> process&;

		auto list_remotes(std::function<void(const std::vector<remote_ptr>&)>&&) -> process&;

		auto list_remotes() -> process&;

		auto delete_remote(const entitie::remote& remote) -> process&;

		auto config() -> process&;

		auto lsjson(const entitie::remote& remote) -> process&;

		auto lsjson(const entitie::file& file) -> process&;

		/**
		 * \brief Lists the objects in the source path to standard output in a human readable format with size and path. Recurses by default.
		 * \param file
		 * \return
		 */
		auto ls(const entitie::file& file) -> process&;

		auto lsl(const entitie::file& file) -> process&;

		auto lsd(const entitie::file& file) -> process&;

		auto lsf(const entitie::file& file) -> process&;

		auto copy_to(const entitie::file& source, const entitie::file& destination) -> process&;

		auto move_to(const entitie::file& source, const entitie::file& destination) -> process&;

		auto delete_file(const entitie::file& file) -> process&;

		auto mkdir(const entitie::file& file) -> process&;

		auto cat(const entitie::file& file) -> process&;

		auto about(const entitie::remote& remote) -> process&;

		auto size(const entitie::file& file) -> process&;

		auto tree(const entitie::file& file) -> process&;

		auto bi_sync(const entitie::file& source, const entitie::file& destination) -> process&;

		auto clean_up(const entitie::remote& remote) -> process&;

		auto copy_url(const std::string &url, const entitie::file& destination) -> process&;


        /**
         * @brief Compare the source and destination and check if the files are the same
         */
		auto check(const entitie::file& source, const entitie::file& destination) -> process&;

		auto add_option(const option& option) -> process&;

		template<class... Args>
		auto add_option(const option& option1, Args&&... args) -> process&
		{
			add_option(option1);
			add_option(std::forward<Args>(args)...);
			return *this;
		}

		static void add_global_option(const option& option);

		template<class... Args>
		static void add_global_option(const option& option1, Args&&... args)
		{
			add_global_option(option1);
			add_global_option(std::forward<Args>(args)...);
		}

	private:
		static std::string _path_rclone;
		static bool _is_initialized;
		static option::vector _global_options;

		std::mutex _mutex;
		std::condition_variable _cv;

		state _state{state::not_launched};

		boost::process::child _child;
		std::unique_ptr<boost::process::ipstream> _out;
		std::unique_ptr<boost::process::ipstream> _err;
		std::unique_ptr<boost::process::opstream> _in;

		boost::asio::thread_pool _pool{5};

		std::vector<std::string> _args;
		std::vector<std::string> _output;
		std::vector<std::string> _error;

		option::vector _local_options;

		auto read_output() -> void;

		auto read_error() -> void;

		std::unique_ptr<boost::signals2::signal<void(const std::string& line)>> _signal_every_line;
		std::unique_ptr<boost::signals2::signal<void(int)>> _signal_finish;
    };
} // namespace iridium::rclone
