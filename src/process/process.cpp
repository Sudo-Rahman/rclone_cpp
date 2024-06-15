#include <memory>
#include <process.hpp>
#include <parsers.hpp>
#include "process_impl.cpp"
#include <config_create.hpp>

#if defined(_WIN32)
#include <boost/process/windows.hpp>
#endif

namespace iridium::rclone
{
	namespace bp = boost::process;
	std::string process::_path_rclone;
	bool process::_is_initialized = false;
	const std::string process::endl = "\n";
	std::vector<option::basic_opt_uptr> process::_global_options = {};
	using namespace entities;

	process::process(process &&p) noexcept
	{
		this->_impl = p._impl;
		p._impl = nullptr;
	}

	auto process::operator=(process &&p) noexcept -> process&
	{
		if (this == &p) return *this;
		delete _impl;
		this->_impl = p._impl;
		p._impl = nullptr;
		return *this;
	}

	auto process::initialize(const std::string &path_rclone) -> bool
	{
		auto is_ok = false;
		if (path_rclone.empty())
		{
#if defined(_WIN32)
			auto path = bp::search_path("rclone.exe");
#else
			auto path = bp::search_path("rclone");
#endif
			if (path.empty())
				throw std::runtime_error("rclone not found in the path");
			_path_rclone = path.string();
		}
		else _path_rclone = path_rclone;

		if (boost::filesystem::exists(_path_rclone))
			is_ok = true;

		_is_initialized = true;
		return is_ok;
	}

	process::process()
	{
		if (!_is_initialized)
			throw std::runtime_error("process not initialized");
		_impl = new _process_impl_();
	}

	auto process::wait_for_start() -> process&
	{
		_impl->wait_for_start();
		return *this;
	}

	auto process::wait_for_finish() -> process&
	{
		_impl->wait_for_finish();
		return *this;
	}

	auto process::close_input_pipe() -> process&
	{
		_impl->close_input_pipe();
		return *this;
	}

	auto process::close_output_pipe() -> process&
	{
		_impl->close_output_pipe();
		return *this;
	}

	auto process::close_error_pipe() -> process&
	{
		_impl->close_error_pipe();
		return *this;
	}

	auto process::exit_code() const -> int { return _impl->_child.exit_code(); }
	auto process::get_state() const -> state { return _impl->_state; }
	auto process::is_running() const -> bool { return _impl->_state == running; }
	auto process::get_output() const -> std::vector<std::string> { return _impl->_output; }
	auto process::get_error() const -> std::vector<std::string> { return _impl->_error; }

	auto process::clean_data() const -> void
	{
		_impl->_output.clear();
		_impl->_error.clear();
	}

	auto process::options() const -> std::vector<option::basic_opt_uptr>& { return _impl->_local_options; }

	auto process::write_input(const std::string &input) const -> void { _impl->write_input(input); }

	auto process::execute(bool with_global_opt) -> process&
	{
		_impl->use_global_options = with_global_opt;
		_impl->execute();
		return *this;
	}

	auto process::with_global_options() -> process&
	{
		_impl->use_global_options = true;
		return *this;
	}

	auto process::commands() const -> std::string { return _impl->commands(); }

	auto process::stop() const -> void { _impl->stop(); }

	auto process::operator<<(const std::string &input) -> process&
	{
		write_input(input);
		return *this;
	}

	process::~process() noexcept { delete _impl; }

	auto process::every_line(std::function<void(const std::string &)> &&callback) -> process&
	{
		_impl->_signal_every_line->connect(
				[this, callback = std::move(callback)](const std::string &line) { callback(line); }
			);
		return *this;
	}

	template<class ...Ts>
	struct overloaded : Ts ...
	{
		using Ts::operator() ...;
	};

	template<class ...Ts>
	overloaded(Ts ...) -> overloaded<Ts ...>;

	auto process::on_finish(on_finish_callback &&callback) -> process&
	{
		_impl->_signal_finish->connect(
				[this, callback = std::move(callback)](const int &exit_code)
				{
					std::visit(overloaded{
							           [](const std::function<void()> &callback) { callback(); },
							           [exit_code](const std::function<void(int)> &callback) { callback(exit_code); },
							           [exit_code, this](const std::function<void(int, process *)> &callback)
							           {
								           callback(exit_code, this);
							           }
					           }, callback);
				}
			);
		return *this;
	}

	auto process::on_stop(std::function<void()> &&callback) -> process&
	{
		_impl->_signal_stop->connect(
				[this, callback = std::move(callback)]() { callback(); }
			);
		return *this;
	}

	auto process::on_start(std::function<void()> &&callback) -> process&
	{
		_impl->_signal_start->connect(
				[this, callback = std::move(callback)]() { callback(); }
			);
		return *this;
	}

	auto process::on_finish_error(std::function<void()> &&callback) -> process&
	{
		_impl->_signal_finish->connect(
				[this, callback = std::move(callback)](const int &exit_code) { if (exit_code not_eq 0) callback(); }
			);
		return *this;
	}

	auto process::version() -> process&
	{
		_impl->_args = {"version"};
		return *this;
	}

	auto process::list_remotes(std::function<void(const std::vector<remote_ptr> &)> &&callback) -> process&
	{
		_impl->_args = {"listremotes", "--long"};
		on_finish([this, callback = std::move(callback)](int exit_code)
		{
			if (exit_code not_eq 0) throw std::runtime_error("error in listremotes");
			auto remotes = std::vector<remote_ptr>();
			auto parser = parser::remote_parser(
				[&remotes](const remote &remote) { remotes.push_back(std::make_shared<entities::remote>(remote)); });
			for (const auto &line: _impl->_output)
				parser.parse(line);
			callback(remotes);
		});
		return *this;
	}

	auto process::list_remotes() -> process&
	{
		_impl->_args = {"listremotes", "--long"};
		return *this;
	}

	auto process::delete_remote(const remote &remote) -> process&
	{
		_impl->_args = {"config", "delete", remote.name()};
		return *this;
	}

	auto process::config() -> process&
	{
		_impl->_args = {"config"};
		return *this;
	}

	auto process::config_create() -> rclone::config_create { return rclone::config_create(this); }

	auto process::lsjson(const remote &remote) -> process&
	{
		_impl->_args = {"lsjson", remote.full_path()};
		return *this;
	}

	auto process::lsjson(const file &file) -> process&
	{
		if (not file.is_dir()) throw std::runtime_error("file is not a directory");
		_impl->_args = {"lsjson", file.absolute_path()};
		return *this;
	}

	auto process::ls(const file &file) -> process&
	{
		if (not file.is_dir()) throw std::runtime_error("file is not a directory");
		_impl->_args = {"ls", file.absolute_path()};
		return *this;
	}

	auto process::lsl(const file &file) -> process&
	{
		if (not file.is_dir()) throw std::runtime_error("file is not a directory");
		_impl->_args = {"lsl", file.absolute_path()};
		return *this;
	}

	auto process::lsd(const file &file) -> process&
	{
		if (not file.is_dir()) throw std::runtime_error("file is not a directory");
		_impl->_args = {"lsd", file.absolute_path()};
		return *this;
	}

	auto process::lsf(const file &file) -> process&
	{
		if (not file.is_dir()) throw std::runtime_error("file is not a directory");
		_impl->_args = {"lsf", file.absolute_path()};
		return *this;
	}

	auto process::copy_to(const file &source, const file &destination) -> process&
	{
		_impl->_args = {"copyto", source.absolute_path(), destination.absolute_path()};
		return *this;
	}

	auto process::move_to(const file &source, const file &destination) -> process&
	{
		_impl->_args = {"moveto", source.absolute_path(), destination.absolute_path()};
		return *this;
	}

	auto process::delete_file(const file &file) -> process&
	{
		_impl->_args = {"delete", file.absolute_path()};
		return *this;
	}

	auto process::rmdir(const file &file) -> process&
	{
		_impl->_args = {"rmdir", file.absolute_path()};
		return *this;
	}

	auto process::rmdirs(const file &file) -> process&
	{
		_impl->_args = {"rmdirs", file.absolute_path()};
		return *this;
	}

	auto process::purge(const file &file) -> process&
	{
		_impl->_args = {"purge", file.absolute_path()};
		return *this;
	}

	auto process::mkdir(const file &file) -> process&
	{
		_impl->_args = {"mkdir", file.absolute_path()};
		return *this;
	}

	auto process::cat(const file &file) -> process&
	{
		_impl->_args = {"cat", file.absolute_path()};
		return *this;
	}

	auto process::about(const remote &remote) -> process&
	{
		_impl->_args = {"about", remote.full_path()};
		return *this;
	}

	auto process::size(const file &file) -> process&
	{
		_impl->_args = {"size", file.absolute_path()};
		return *this;
	}

	auto process::tree(const file &file) -> process&
	{
		_impl->_args = {"tree", file.absolute_path()};
		return *this;
	}

	auto process::bi_sync(const file &source, const file &destination) -> process&
	{
		_impl->_args = {"bisync", source.absolute_path(), destination.absolute_path()};
		return *this;
	}

	auto process::sync(const file &source, const file &destination) -> process&
	{
		_impl->_args = {"sync", source.absolute_path(), destination.absolute_path()};
		return *this;
	}

	auto process::clean_up(const remote &remote) -> process&
	{
		_impl->_args = {"cleanup", remote.root_path()};
		return *this;
	}

	auto process::copy_url(const std::string &url, const file &destination) -> process&
	{
		_impl->_args = {"copyurl", url, destination.absolute_path()};
		return *this;
	}

	auto process::check(const file &source, const file &destination) -> process&
	{
		if (not source.is_dir() or not destination.is_dir())
			throw std::runtime_error("source or destination is not a directory");
		_impl->_args = {
						"check", source.absolute_path(), destination.absolute_path()
				};
		using option::logging::log_level;
		add_option(option::logging::use_json_log(), log_level::uptr(log_level::info));
		return *this;
	}

	auto process::touch(const entities::file &file) -> process&
	{
		_impl->_args = {"touch", file.absolute_path()};
		return *this;
	}

	void process::add_option(std::vector<option::basic_opt_uptr> &&opts) const
	{
		for (auto &opt: opts)
			_impl->_local_options.push_back(std::move(opt));
	}

	void process::clear_global_options() { _global_options.clear(); }

	auto process::join_vector(const std::vector<std::string> &vec) -> std::string
	{
		return boost::algorithm::join(vec, endl);
	}
} // namespace iridium::rclone
