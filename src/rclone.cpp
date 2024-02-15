#include "rclone.hpp"
#include <iostream>
#include <regex>
#include <boost/json.hpp>


namespace Iridium
{
    namespace ba = boost::asio;
    namespace bp = boost::process;
    namespace bs2 = boost::signals2;
    namespace bjson = boost::json;
    std::string rclone::_path_rclone;
    bool rclone::_is_initialized = false;
    const std::string rclone::endl = "\n";


    void rclone::initialize(const std::string &path_rclone)
    {
        _path_rclone = path_rclone;
        _is_initialized = true;
    }

    rclone::rclone()
    {
        if (!_is_initialized)
            throw std::runtime_error("rclone not initialized");
        _signal_every_line = std::make_unique<bs2::signal<void(const std::string &)>>();
        _signal_finish = std::make_unique<bs2::signal<void(int)>>();
    }

    rclone &rclone::wait_for_start()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _cv.wait(lock, [this]
        { return _state == state::launched; });
        return *this;
    }

    rclone &rclone::wait_for_finish()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _cv.wait(lock, [this]
        { return _state == state::finished; });
        return *this;
    }

    int rclone::exit_code() const
    {
        return _child.exit_code();
    }

    void rclone::write_input(const std::string &input)
    {
        if (_state != state::launched)
            throw std::runtime_error("rclone not started");

        if (_in->pipe().is_open())
            _in->pipe().write(input.c_str(), input.size());
    }

    void rclone::read_output()
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

    void rclone::read_error()
    {
        std::string line;
        while (std::getline(*_err, line))
        {
            _error.emplace_back(line);
            std::cerr << line << std::endl;
        }
        _err.reset();
    }

    rclone &rclone::execute()
    {
        if (_state != state::not_launched)
            throw std::runtime_error("rclone already started");

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
            _state = state::finished;
            if (_signal_finish != nullptr)
                (*_signal_finish)(_child.exit_code());
            _in.reset();
            _cv.notify_all();

        });

        return *this;

    }

    void rclone::stop()
    {
        _in.reset();
        _pool.stop();
        _child.terminate();
        _state = state::stopped;
    }

    rclone &rclone::operator<<(const std::string &input)
    {
        write_input(input);
        return *this;
    }


    rclone::~rclone()
    {
        if (_state == state::launched)
        {
//            if the child waiting for an input, close it
            _in->pipe().close();
            std::cerr << "rclone are destroyed without being stopped" << std::endl;
        }

    }

    rclone &rclone::every_line(const std::function<void(const std::string &)> &&callback)
    {

        _signal_every_line->connect(
                [this, callback](const std::string &line)
                {
                    callback(line);
                }
        );
        return *this;
    }

    rclone &rclone::finished(const std::function<void(int)> &&callback)
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

    rclone &rclone::version()
    {
        _args = {"version"};
        return *this;
    }

    rclone &rclone::list_remotes(std::vector<rclone_remote> &remotes)
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
                                remotes.emplace_back(match[1], rclone_remote::string_to_remote_type.at(match[2]), "");
                            }
                        }
                    });
                }
        );
        return *this;
    }

    rclone &rclone::delete_remote(const rclone_remote &remote)
    {
        _args = {"config", "delete", remote.name()};
        return *this;
    }


    rclone &rclone::config()
    {
        _args = {"config", "show"};
        return *this;
    }

    rclone &rclone::lsjson(const rclone_remote &remote)
    {
        _args = {"lsjson", remote.full_path()};
        return *this;
    }

    rclone &rclone::lsjson(rclone_file &file)
    {
        _args = {"lsjson", file.absolute_path()};

        every_line(
                [&file](const std::string &line)
                {
                    rclone_file::from_json(line, &file);
                }
        );

        return *this;
    }

    rclone &rclone::lsjson(rclone_file &file, const std::function<void(rclone_file child)> &&callback)
    {
        _args = {"lsjson", file.absolute_path()};

        every_line(
                [&file, callback](const std::string &line)
                {
                    auto child = rclone_file::from_json(line, &file);
                    if (child)
                        callback(*child);
                }
        );

        return *this;
    }

    rclone &rclone::copy_to(const rclone_file &source, const rclone_file &destination)
    {
        _args = {"copyto", source.absolute_path(), destination.absolute_path()};
        return *this;
    }

    rclone &rclone::move_to(const rclone_file &source, const rclone_file &destination)
    {
        _args = {"moveto", source.absolute_path(), destination.absolute_path()};
        return *this;
    }

    rclone &rclone::delete_file(const rclone_file &file)
    {
        _args = {"delete", file.absolute_path()};
        return *this;
    }

    rclone &rclone::mkdir(const rclone_file &file)
    {
        _args = {"mkdir", file.absolute_path()};
        return *this;
    }

    rclone &rclone::cat(const Iridium::rclone_file &file)
    {
        _args = {"cat", file.absolute_path()};
        return *this;
    }

    rclone &rclone::about(const rclone_remote &remote)
    {
        _args = {"about", remote.root_path()};
        return *this;
    }

    rclone &rclone::size(const rclone_remote &remote){
        _args = {"size", remote.root_path()};
        return *this;
    }

} // namespace Iridium