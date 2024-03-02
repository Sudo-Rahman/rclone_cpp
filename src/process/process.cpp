#include <process.hpp>
#include <iostream>
#include <boost/json.hpp>
#include <parsers.hpp>


namespace iridium::rclone
{
	namespace ba = boost::asio;
	namespace bp = boost::process;
	namespace bs2 = boost::signals2;
	std::string process::_path_rclone;
	bool process::_is_initialized = false;
	const std::string process::endl = "\n";
	option::vector process::_global_options = {};


	auto process::initialize(const std::string& path_rclone) -> void
	{
		if (path_rclone.empty())
		{
			auto path = bp::search_path("rclone");
			if (path.empty())
				throw std::runtime_error("rclone not found in the path");
			_path_rclone = path.string();
		}
		else _path_rclone = path_rclone;
		_is_initialized = true;
	}

	process::process()
	{
		if (!_is_initialized)
			throw std::runtime_error("process not initialized");
		_signal_every_line = std::make_unique<bs2::signal<void(const std::string&)>>();
		_signal_finish = std::make_unique<bs2::signal<void(int)>>();
	}

	auto process::wait_for_start() -> process&
	{
		std::unique_lock lock(_mutex);
		_cv.wait(lock, [this] { return _state == state::running; });
		return *this;
	}

	auto process::wait_for_finish() -> process&
	{
		std::unique_lock lock(_mutex);
		_cv.wait(lock, [this] { return _state == state::finished; });
		_pool.join();
		return *this;
	}

	auto process::close_input_pipe() -> process&
	{
        if (_in and _in->pipe().is_open())
            _in->pipe().close();
        else  std::cerr << "input pipe is not open or nullptr" << std::endl;
		return *this;
	}

	auto process::close_output_pipe() -> process&
	{
		if (_out and _out->pipe().is_open())
            _out->pipe().close();
        else std::cerr << "output pipe is not open or nullptr" << std::endl;
		return *this;
	}

	auto process::close_error_pipe() -> process&
	{
		if (_err and _err->pipe().is_open())
            _err->pipe().close();
        else std::cerr << "error pipe is not open or nullptr" << std::endl;
		return *this;
	}

	auto process::exit_code() const -> int { return _child.exit_code(); }

	auto process::write_input(const std::string& input) const -> void
	{
		if (_state != state::running)
			throw std::runtime_error("process not started");

		if (_in->pipe().is_open())
			_in->pipe().write(input.c_str(), input.size());
	}

	auto process::read_output() -> void
	{
		std::string line;
		while (std::getline(*_out, line))
		{
			_output.emplace_back(line);
			if (_signal_every_line != nullptr)
				(*_signal_every_line)(line);
		}
		_out.reset();
	}

	auto process::read_error() -> void
	{
		std::string line;
		while (std::getline(*_err, line))
		{
			_error.emplace_back(line);
			std::cerr << line << std::endl;
		}
		_err.reset();
	}

	auto process::execute() -> process&
	{
		if (_state != state::not_launched)
			throw std::runtime_error("process already started");

		option::add_options_to_vector(_global_options, _args);
		option::add_options_to_vector(_local_options, _args);

		try
		{
			_in = std::make_unique<bp::opstream>();
			_out = std::make_unique<bp::ipstream>();
			_err = std::make_unique<bp::ipstream>();
			_child = bp::child(
				_path_rclone, bp::args(_args),
				bp::std_in < *_in, bp::std_out > *_out,
				bp::std_err > *_err);
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
			exit(1);
		}

		_state = state::running;
		_cv.notify_all();

		ba::post(_pool, [this] { read_output(); });
		ba::post(_pool, [this] { read_error(); });

		ba::post(_pool, [this]
		{
            if (_child.running())
                _child.wait();
			if (_child.exit_code() == 0)
				_state = state::finished;
			else _state = state::error;
			if (_signal_finish != nullptr)
				(*_signal_finish)(_child.exit_code());
			_cv.notify_all();
		});

		return *this;
	}

	auto process::stop() -> void
	{
        std::cout << _child.running() << " " << (_state == state::running) << std::endl;
        if (not is_running()) throw std::runtime_error("process not started");
		close_input_pipe();
        _pool.stop();
        _pool.join();
        _child.terminate();
        _state = state::stopped;
        _cv.notify_all();
	}

	auto process::operator<<(const std::string& input) -> process&
	{
		write_input(input);
		return *this;
	}


	process::~process()
	{
		if (_state == state::running)
		{

            std::cerr << "process are destroyed without being stopped" << std::endl;
            process::stop();
		}
	}

	auto process::every_line(std::function<void(const std::string&)>&& callback) -> process&
	{
		_signal_every_line->connect(
			[this, callback](const std::string& line) { ba::post(_pool, [callback, line] { callback(line); }); }
		);
		return *this;
	}

	auto process::on_finish(std::function<void(int)>&& callback) -> process&
	{
		_signal_finish->connect(
			[this, callback](const int& exit_code) { ba::post(_pool, [callback, exit_code] { callback(exit_code); }); }
		);
		return *this;
	}

	auto process::on_finish_error(std::function<void()>&& callback) -> process&
	{
		_signal_finish->connect(
			[this, callback](const int& exit_code)
			{
				if (exit_code not_eq 0)
					ba::post(_pool, [&callback] { callback(); });
			}
		);
		return *this;
	}

	auto process::version() -> process&
	{
		_args = {"version"};
		return *this;
	}

	auto process::list_remotes(std::function<void(const std::vector<remote_ptr>&)>&& callback) -> process&
	{
		_args = {"listremotes", "--long"};
        on_finish([this, callback](int exit_code)
		{
			if (exit_code not_eq 0) throw std::runtime_error("error in listremotes");
			auto remotes = std::vector<remote_ptr>();
			auto parser = parser::remote_parser(
				[&remotes](const entitie::remote& remote)
				{
					remotes.push_back(std::make_shared<entitie::remote>(remote));
				});
			for (const auto& line: _output)
				parser.parse(line);
			callback(remotes);
		});
		return *this;
	}

	auto process::list_remotes() -> process&
	{
		_args = {"listremotes", "--long"};
		return *this;
	}

	auto process::delete_remote(const entitie::remote& remote) -> process&
	{
		_args = {"config", "delete", remote.name()};
		return *this;
	}


	auto process::config() -> process&
	{
		_args = {"config"};
		return *this;
	}

	auto process::lsjson(const entitie::remote& remote) -> process&
	{
		_args = {"lsjson", remote.full_path()};
		return *this;
	}

	auto process::lsjson(const entitie::file& file) -> process&
	{
		if (not file.is_dir()) throw std::runtime_error("file is not a directory");
		_args = {"lsjson", file.absolute_path()};
		return *this;
	}

	auto process::ls(const entitie::file& file) -> process&
	{
		if (not file.is_dir()) throw std::runtime_error("file is not a directory");
		_args = {"ls", file.absolute_path()};
		return *this;
	}

	auto process::lsl(const entitie::file& file) -> process&
	{
		if (not file.is_dir()) throw std::runtime_error("file is not a directory");
		_args = {"lsl", file.absolute_path()};
		return *this;
	}

	auto process::lsd(const entitie::file& file) -> process&
	{
		if (not file.is_dir()) throw std::runtime_error("file is not a directory");
		_args = {"lsd", file.absolute_path()};
		return *this;
	}

	auto process::lsf(const entitie::file& file) -> process&
	{
		if (not file.is_dir()) throw std::runtime_error("file is not a directory");
		_args = {"lsf", file.absolute_path()};
		return *this;
	}


	auto process::copy_to(const entitie::file& source, const entitie::file& destination) -> process&
	{
		_args = {"copyto", source.absolute_path(), destination.absolute_path()};
		return *this;
	}

	auto process::move_to(const entitie::file& source, const entitie::file& destination) -> process&
	{
		_args = {"moveto", source.absolute_path(), destination.absolute_path()};
		return *this;
	}

	auto process::delete_file(const entitie::file& file) -> process&
	{
		_args = {"delete", file.absolute_path()};
		return *this;
	}

	auto process::mkdir(const entitie::file& file) -> process&
	{
		_args = {"mkdir", file.absolute_path()};
		return *this;
	}

	auto process::cat(const entitie::file& file) -> process&
	{
		_args = {"cat", file.absolute_path()};
		return *this;
	}

	auto process::about(const entitie::remote& remote) -> process&
	{
		_args = {"about", remote.root_path()};
		return *this;
	}

	auto process::size(const entitie::file& file) -> process&
	{
		_args = {"size", file.absolute_path()};
		return *this;
	}

	auto process::tree(const entitie::file& file) -> process&
	{
		_args = {"tree", file.absolute_path()};
		return *this;
	}

	auto process::bi_sync(const entitie::file& source, const entitie::file& destination) -> process&
	{
		_args = {"bisync", source.absolute_path(), destination.absolute_path()};
		return *this;
	}

	auto process::clean_up(const entitie::remote& remote) -> process&
	{
		_args = {"cleanup", remote.root_path()};
		return *this;
	}
	auto process::copy_url(const std::string& url, const entitie::file& destination) -> process&
	{
		_args = {"copyurl", url, destination.absolute_path()};
		return *this;
	}

	auto process::check(const entitie::file & source, const entitie::file & destination) -> process &
	{
		if(not source.is_dir() or not destination.is_dir()) throw std::runtime_error("source or destination is not a directory");
		_args = {"check", source.absolute_path(), destination.absolute_path()};
		return *this;
	}

	auto process::add_option(const option& option) -> process&
	{
		_local_options.push_back(option);
		return *this;
	}

	void process::add_global_option(const option& option) { process::_global_options.push_back(option); }
} // namespace iridium::rclone
