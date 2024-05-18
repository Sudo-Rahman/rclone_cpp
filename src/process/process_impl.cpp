#include <memory>
#include <process.hpp>
#include <iostream>
#include <parsers.hpp>
#include <config_create.hpp>
#include <string>
#include <boost/process.hpp>
#include <boost/asio.hpp>
#include <functional>
#include "../entities.hpp"
#include "../options.hpp"
#include <boost/signals2.hpp>
#include <boost/algorithm/string/join.hpp>

#if defined(_WIN32)
#include <boost/process/windows.hpp>
#endif

namespace ba = boost::asio;
namespace bp = boost::process;
namespace bs2 = boost::signals2;
using namespace iridium::rclone::entities;

namespace iridium::rclone
{
	class process::_process_impl_
	{
	public:
		friend class process;
		friend class config_create;

	private:
		std::mutex _mutex;
		std::condition_variable _cv;

		boost::process::child _child;
		std::unique_ptr<boost::process::ipstream> _out;
		std::unique_ptr<boost::process::ipstream> _err;
		std::unique_ptr<boost::process::opstream> _in;

		/// 1 thread for wait child, 1 thread for reading output,
		/// 1 thread for reading error and 2 last threads computing the signals for every line and finish
		boost::asio::thread_pool _pool{5};
		std::atomic_uint8_t _counter_read{0};

		state _state{state::not_launched};

		std::vector<std::string> _args;
		std::vector<std::string> _output;
		std::vector<std::string> _error;

		std::vector<option::basic_opt_uptr> _local_options;

		std::unique_ptr<boost::signals2::signal<void(const std::string &line)>> _signal_every_line;
		std::unique_ptr<boost::signals2::signal<void(int)>> _signal_finish;
		std::unique_ptr<boost::signals2::signal<void()>> _signal_start;
		std::unique_ptr<boost::signals2::signal<void()>> _signal_stop;

		_process_impl_()
		{
			_signal_every_line = std::make_unique<bs2::signal<void(const std::string &)>>();
			_signal_finish = std::make_unique<bs2::signal<void(int)>>();
			_signal_start = std::make_unique<bs2::signal<void()>>();
			_signal_stop = std::make_unique<bs2::signal<void()>>();
		}

		auto wait_for_start() -> void
		{
			std::unique_lock lock(_mutex);
			_cv.wait(lock, [this] { return _state == state::running; });
		}

		auto wait_for_finish() -> void { _pool.join(); }

		auto close_input_pipe() -> void
		{
			if (_in and _in->pipe().is_open())
				_in->pipe().close();
			else std::cerr << "input pipe is not open or nullptr" << std::endl;
		}

		auto close_output_pipe() -> void
		{
			if (_out and _out->pipe().is_open())
				_out->pipe().close();
			else std::cerr << "output pipe is not open or nullptr" << std::endl;
		}

		auto close_error_pipe() -> void
		{
			if (_err and _err->pipe().is_open())
				_err->pipe().close();
			else std::cerr << "error pipe is not open or nullptr" << std::endl;
		}

		[[nodiscard]] auto is_running() const -> bool { return _state == state::running; }

		auto write_input(const std::string &input) const -> void
		{
			if (_state != state::running)
				throw std::runtime_error("process not started");

			if (_in->pipe().is_open())
				_in->pipe().write(input.c_str(), input.size());
		}

		auto read_output() -> void
		{
			std::string line;
			while (std::getline(*_out, line))
			{
				_output.emplace_back(line);
				if (_signal_every_line)
					_signal_every_line->operator()(line);
			}
			_out.reset();
		}

		auto read_error() -> void
		{
			std::string line;
			while (std::getline(*_err, line))
			{
				_error.emplace_back(line);
				_output.emplace_back(line);
				if (_signal_every_line)
					_signal_every_line->operator()(line);
			}
			_err.reset();
		}

		auto execute(bool with_global_option = true) -> void
		{
			if (_state != state::not_launched)
				throw std::runtime_error("process already started");

			if (with_global_option)
				option::basic_option::add_options_to_vector(_global_options, _args);
			option::basic_option::add_options_to_vector(_local_options, _args);

			try
			{
				_in = std::make_unique<bp::opstream>();
				_out = std::make_unique<bp::ipstream>();
				_err = std::make_unique<bp::ipstream>();
				_child = bp::child(
						bp::exe(_path_rclone),
						bp::args(_args),
						bp::std_in < *_in, bp::std_out > *_out,
						bp::std_err > *_err
#if defined(_WIN32)
					, bp::windows::hide
#endif
					);
			}
			catch (const std::exception &e)
			{
				std::cerr << e.what() << std::endl;
				exit(1);
			}

			_state = state::running;
			if (_signal_start)
				_signal_start->operator()();
			_cv.notify_all();

			ba::post(_pool, [this]
			{
				read_output();
				_counter_read++;
			});
			ba::post(_pool, [this]
			{
				read_error();
				_counter_read++;
			});

			ba::post(_pool, [this]
			{
				try
				{
					if (_child.running())
						_child.wait();
				}
				catch (boost::wrapexcept<bp::process_error> &e) { std::cerr << e.what() << std::endl; }
				if (_state not_eq state::stopped)
				{
					if (_child.exit_code() == 0)
						_state = state::finished;
					else _state = state::error;
					while (_counter_read < 2)
						std::this_thread::sleep_for(std::chrono::milliseconds(10));
					if (_signal_finish)
						_signal_finish->operator()(_child.exit_code());
				}
				_cv.notify_all();
			});
		}

		auto commands() const -> std::string
		{
			if (_state not_eq state::not_launched)
				return boost::algorithm::join(_args, " ");
			auto args = _args;
			option::basic_option::add_options_to_vector(_global_options, args);
			option::basic_option::add_options_to_vector(_local_options, args);
			return boost::algorithm::join(args, " ");
		}

		auto stop() -> void
		{
			if (not is_running()) throw std::runtime_error("process not running");
			_state = state::stopped;
			if (_signal_stop)
				_signal_stop->operator()();
			close_input_pipe();
			_child.terminate();
			if (is_running())
			{
				_pool.stop();
				_pool.join();
			}
			_cv.notify_all();
		}

		~_process_impl_() noexcept
		{
			if (_state == state::running)
			{
				stop();
				std::cerr << "process are destroyed without being stopped" << std::endl;
			}
		}

		auto every_line(std::function<void(const std::string &)> &&callback) -> void
		{
			_signal_every_line->connect(
					[this, callback](const std::string &line) { ba::post(_pool, [callback, line] { callback(line); }); }
				);
		}

		auto on_finish(std::function<void(int)> &&callback) -> void
		{
			_signal_finish->connect(
					[this, callback = std::move(callback)](const int &exit_code)
					{
						ba::post(_pool, [callback, exit_code] { callback(exit_code); });
					}
				);
		}

		auto on_stop(std::function<void()> &&callback) -> void
		{
			_signal_stop->connect(
					[this, callback = std::move(callback)]() { ba::post(_pool, [callback] { callback(); }); }
				);
		}

		auto on_start(std::function<void()> &&callback) -> void
		{
			_signal_start->connect(
					[this, callback = std::move(callback)]() { ba::post(_pool, [callback] { callback(); }); }
				);
		}

		auto on_finish_error(std::function<void()> &&callback) -> void
		{
			_signal_finish->connect(
					[this, callback = std::move(callback)](const int &exit_code)
					{
						if (exit_code not_eq 0)
							ba::post(_pool, [&callback] { callback(); });
					}
				);
		}

		auto list_remotes(std::function<void(const std::vector<remote_ptr> &)> &&callback) -> void
		{
			_args = {"listremotes", "--long"};
			on_finish([this, callback = std::move(callback)](int exit_code)
			{
				if (exit_code not_eq 0) throw std::runtime_error("error in listremotes");
				auto remotes = std::vector<remote_ptr>();
				auto parser = parser::remote_parser(
					[&remotes](const remote &remote)
					{
						remotes.push_back(std::make_shared<entities::remote>(remote));
					});
				for (const auto &line: _output)
					parser.parse(line);
				callback(remotes);
			});
		}
	};
}
