#pragma once

#include <any>
#include <string>
#include <functional>
#include "../entities.hpp"
#include "../options.hpp"
#include "../parsers.hpp"

namespace iridium::rclone
{
	class config_create;

	class process
	{
	public:
		process();

		~process() noexcept;

		process(const process &) = delete;

		process(process &&) noexcept;

		auto operator=(const process &) -> process&;

		auto operator=(process &&) noexcept -> process&;

		enum state
		{
			not_launched,
			running,
			finished,
			stopped,
			error
		};

		/**
		 * @brief Initialize the rclone process
		 * @param path_rclone the path to the rclone executable or if empty the system path will be used
		 * @return return true if rclone is found
		 */
		static auto initialize(const std::string &path_rclone = "") -> bool;

		auto wait_for_start() -> process&;

		auto wait_for_finish() -> process&;

		auto close_input_pipe() -> process&;

		auto close_output_pipe() -> process&;

		auto close_error_pipe() -> process&;

		auto execute(bool with_global_opt = false) -> process&;

		auto with_global_options() -> process&;

		auto get() -> process * { return this; };

		[[nodiscard]] auto exit_code() const -> int;

		[[nodiscard]] auto get_state() const -> state;

		[[nodiscard]] auto is_running() const -> bool;

		[[nodiscard]] auto get_output() const -> std::vector<std::string>;

		[[nodiscard]] auto get_error() const -> std::vector<std::string>;

		[[nodiscard]] auto options() const -> std::vector<option::basic_opt_uptr>&;

		[[nodiscard]] static auto global_options() -> std::vector<option::basic_opt_uptr>& { return _global_options; }

		[[nodiscard]] auto commands() const -> std::string;

		auto stop() const -> void;

		static const std::string endl;

		auto operator<<(const std::string &input) -> process&;

		auto write_input(const std::string &input) const -> void;

		auto every_line(std::function<void(const std::string &)> &&callback) -> process&;

		template<class T>
		auto every_line_parser(std::shared_ptr<parser::basic_parser<T>> parser) -> process& requires(std::is_base_of_v<
			entity, T>)
		{
			every_line([this, parser = std::move(parser)](const std::string &line) { parser->parse(line); });
			return *this;
		}

		auto on_finish(std::function<void(int)> &&callback) -> process&;

		auto on_stop(std::function<void()> &&callback) -> process&;

		auto on_start(std::function<void()> &&callback) -> process&;

		template<class T>
		auto on_finish_parser(std::shared_ptr<parser::basic_parser<T>> parser) -> process& requires(std::is_base_of_v<
			entity, T>)
		{
			on_finish([this,parser = std::move(parser)](int) { parser->parse(join_vector(get_output())); });
			return *this;
		}

		auto on_finish_error(std::function<void()> &&callback) -> process&;

		auto version() -> process&;

		auto list_remotes(std::function<void(const std::vector<entities::remote_ptr> &)> &&) -> process&;

		auto list_remotes() -> process&;

		auto delete_remote(const entities::remote &remote) -> process&;

		auto config() -> process&;

		auto config_create() -> config_create;

		friend class config_create;

		auto lsjson(const entities::remote &remote) -> process&;

		auto lsjson(const entities::file &file) -> process&;

		/**
		 * \brief Lists the objects in the source path to standard output in a human readable format with size and path. Recurses by default.
		 * \param file
		 * \return
		 */
		auto ls(const entities::file &file) -> process&;

		auto lsl(const entities::file &file) -> process&;

		auto lsd(const entities::file &file) -> process&;

		auto lsf(const entities::file &file) -> process&;

		auto copy_to(const entities::file &source, const entities::file &destination) -> process&;

		auto move_to(const entities::file &source, const entities::file &destination) -> process&;

		auto delete_file(const entities::file &file) -> process&;

		auto rmdir(const entities::file &file) -> process&;

		/**
		 * @brief Remove empty directories under the path.
		 * @details This recursively removes any empty directories (including directories that only contain empty directories),
		 * that it finds under the path. The root path itself will also be removed if it is empty, unless you supply the --leave-root flag.
		 * @param file
		 * @return this
		 */
		auto rmdirs(const entities::file &file) -> process&;

		/**
		 * @brief Remove the path and all of its contents.
		 * @details Remove the path and all of its contents.
		 * Note that this does not obey include/exclude filters - everything will be removed.
		 * Use the delete command if you want to selectively delete files.
		 * To delete empty directories only, use command rmdir or rmdirs.
		 * @param file
		 * @return this
		 */
		auto purge(const entities::file &file) -> process&;

		auto mkdir(const entities::file &file) -> process&;

		auto cat(const entities::file &file) -> process&;

		auto about(const entities::remote &remote) -> process&;

		auto size(const entities::file &file) -> process&;

		auto tree(const entities::file &file) -> process&;

		auto bi_sync(const entities::file &source, const entities::file &destination) -> process&;

		/**
		 * @brief Sync the source to the destination, changing the destination only. Doesn't transfer files that are identical on source and destination, testing by size and modification time or MD5SUM. Destination is updated to match source, including deleting files if necessary (except duplicate objects, see below). If you don't want to delete files from destination, use the copy command instead.
		 * @param source
		 * @param destination
		 * @return this
		 */
		auto sync(const entities::file &source, const entities::file &destination) -> process&;

		auto clean_up(const entities::remote &remote) -> process&;

		auto copy_url(const std::string &url, const entities::file &destination) -> process&;

		/**
		 * @brief Compare the source and destination and check if the files are the same
		 */
		auto check(const entities::file &source, const entities::file &destination) -> process&;

		auto touch(const entities::file &file) -> process&;

		template<class ...Args>
		auto add_option(Args && ...args) -> void requires(std::conjunction_v<std::is_convertible<Args,
			option::basic_opt_uptr> ...>)
		{
			auto vec = std::vector<option::basic_opt_uptr>{};
			(vec.push_back(std::forward<Args>(args)), ...);
			add_option(std::move(vec));
		}

	private:
		void add_option(std::vector<option::basic_opt_uptr> &&) const;

	public:
		template<class ...Args>
		static auto add_global_option(Args && ...args) -> void requires(std::conjunction_v<std::is_convertible<Args,
			option::basic_opt_uptr> ...>) { (_global_options.push_back(std::forward<Args>(args)), ...); }

		static void clear_global_options();

	private:
		static std::string _path_rclone;
		static bool _is_initialized;
		static std::vector<option::basic_opt_uptr> _global_options;

		[[nodiscard]] static auto join_vector(const std::vector<std::string> &vec) -> std::string;

		class _process_impl_;
		_process_impl_ *_impl;
	};

	using process_ptr = std::shared_ptr<process>;
	using process_uptr = std::unique_ptr<process>;
} // namespace iridium::rclone
