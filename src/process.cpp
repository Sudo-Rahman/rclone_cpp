#include <process.hpp>
#include <iostream>
#include <regex>
#include <boost/json.hpp>
#include <boost/algorithm/string/join.hpp>


namespace Iridium::rclone
{
    namespace ba = boost::asio;
    namespace bp = boost::process;
    namespace bs2 = boost::signals2;
    namespace bjson = boost::json;
    std::string process::_path_rclone;
    bool process::_is_initialized = false;
    const std::string process::endl = "\n";
    option::vector process::_global_options = {};


    void process::initialize(const std::string &path_rclone)
    {
        if (path_rclone == "")
        {
            auto path = bp::search_path("rclone");
            if (path.empty())
                throw std::runtime_error("rclone not found in the path");
            _path_rclone = path.string();
        } else _path_rclone = path_rclone;
        _is_initialized = true;
    }

    process::process()
    {
        if (!_is_initialized)
            throw std::runtime_error("process not initialized");
        _signal_every_line = std::make_unique<bs2::signal<void(const std::string &)>>();
        _signal_finish = std::make_unique<bs2::signal<void(int)>>();
    }

    process &process::wait_for_start()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _cv.wait(lock, [this]
        { return _state == state::launched; });
        return *this;
    }

    process &process::wait_for_finish()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _cv.wait(lock, [this]
        { return _state == state::finished; });
        return *this;
    }

    int process::exit_code() const
    {
        return _child.exit_code();
    }

    void process::write_input(const std::string &input)
    {
        if (_state != state::launched)
            throw std::runtime_error("process not started");

        if (_in->pipe().is_open())
            _in->pipe().write(input.c_str(), input.size());
    }

    void process::read_output()
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

    void process::read_error()
    {
        std::string line;
        while (std::getline(*_err, line))
        {
            _error.emplace_back(line);
            std::cerr << line << std::endl;
        }
        _err.reset();
    }

    process &process::execute()
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
        } catch (const std::exception &e)
        {
            std::cerr << e.what() << std::endl;
            exit(1);
        }

        _state = state::launched;
        _cv.notify_all();

        ba::post(_pool, [this]
        {
            read_output();
            _counter++;
        });
        ba::post(_pool, [this]
        {
            read_error();
            _counter++;
        });

        ba::post(_pool, [this]
        {
            _child.wait();
            while (_counter < 2)
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            if (_child.exit_code() == 0)
                _state = state::finished;
            else _state = state::error;
            if (_signal_finish != nullptr)
                (*_signal_finish)(_child.exit_code());
            _in.reset();
            _cv.notify_all();

        });

        return *this;

    }

    void process::stop()
    {
        _in.reset();
        _pool.stop();
        _child.terminate();
        _state = state::stopped;
    }

    process &process::operator<<(const std::string &input)
    {
        write_input(input);
        return *this;
    }


    process::~process()
    {
        if (_state == state::launched)
        {
//            if the child waiting for an input, close it
            _in->pipe().close();
            std::cerr << "process are destroyed without being stopped" << std::endl;
        }

    }

    process &process::every_line(std::function<void(const std::string &)> &&callback)
    {

        _signal_every_line->connect(
                [this, callback](const std::string &line)
                {
                    callback(line);
                }
        );
        return *this;
    }

    process &process::finished(std::function<void(int)> &&callback)
    {
        _signal_finish->connect(
                [this, callback](const int &exit_code)
                {
                    ba::post(_pool, [&callback, exit_code]
                    {
                        callback(exit_code);
                    });
                }
        );
        return *this;
    }

    process &process::finished_error(std::function<void()> &&callback)
    {
        _signal_finish->connect(
                [this, callback](const int &exit_code)
                {
                    if (exit_code not_eq 0)
                        ba::post(_pool, [&callback]
                        {
                            callback();
                        });
                }
        );
        return *this;
    }

    process &process::version()
    {
        _args = {"version"};
        return *this;
    }

    process &process::list_remotes(std::vector<remote> &remotes)
    {
        _args = {"listremotes", "--long"};
        _signal_finish->connect(
                [this, &remotes](const int &exit_code)
                {
                    ba::post(_pool, [this, &remotes]
                    {
                        std::regex re(R"((\w+):\s+(\w+))");
                        std::smatch match;
                        for (const auto &line: _output)
                        {
                            if (std::regex_search(line, match, re))
                            {
                                remotes.emplace_back(match[1], remote::string_to_remote_type.at(match[2]), "");
                            }
                        }
                    });
                }
        );
        return *this;
    }

    process &process::delete_remote(const remote &remote)
    {
        _args = {"config", "delete", remote.name()};
        return *this;
    }


    process &process::config()
    {
        _args = {"config"};
        return *this;
    }

    process &process::lsjson(const remote &remote)
    {
        _args = {"lsjson", remote.full_path()};
        return *this;
    }

    process &process::lsjson(file &file)
    {
        _args = {"lsjson", file.absolute_path()};

        every_line(
                [&file](const std::string &line)
                {
                    file::from_json(line, &file);
                }
        );

        return *this;
    }

    process &process::lsjson(file &file, const std::function<void(Iridium::rclone::file)> &&callback)
    {
        _args = {"lsjson", file.absolute_path()};

        every_line(
                [&file, callback](
                        const std::string &line
                )
                {
                    auto child = file::from_json(line, &file);
                    if (child)
                        callback(*child);
                }
        );

        return *this;
    }

    process &process::copy_to(const file &source, const file &destination)
    {
        _args = {"copyto", source.absolute_path(), destination.absolute_path()};
        return *this;
    }

    process &process::move_to(const file &source, const file &destination)
    {
        _args = {"moveto", source.absolute_path(), destination.absolute_path()};
        return *this;
    }

    process &process::delete_file(const file &file)
    {
        _args = {"delete", file.absolute_path()};
        return *this;
    }

    process &process::mkdir(const file &file)
    {
        _args = {"mkdir", file.absolute_path()};
        return *this;
    }

    process &process::cat(const file &file)
    {
        _args = {"cat", file.absolute_path()};
        return *this;
    }

    process &process::about(const remote &remote, std::function<void(const Iridium::rclone::about &)> &&callback)
    {
        _args = {"about", remote.root_path()};
        finished(
                [this, callback](int exit_code)
                {
                    ba::post(_pool, [this, callback]
                    {
                        auto about = Iridium::rclone::about::create(boost::algorithm::join(_output, "\n"));
                        callback(about);
                    });
                }
        );
        return *this;
    }

    process &process::size(const file &file, std::function<void(const Iridium::rclone::size &)> &&callback)
    {
        _args = {"size", file.absolute_path()};
        finished(
                [this, callback](int exit_code)
                {
                    ba::post(_pool, [this, callback]
                    {
                        auto size = Iridium::rclone::size::create(boost::algorithm::join(_output, "\n"));
                        callback(size);
                    });
                }
        );
        return *this;
    }

    process &process::tree(const file &file)
    {
        _args = {"tree", file.absolute_path()};
        return *this;
    }

    process &process::add_option(const option &option)
    {
        _local_options.push_back(option);
        return *this;
    }

    void process::add_global_option(const option &option)
    {
        process::_global_options.push_back(option);
    }

} // namespace Iridium::rclone