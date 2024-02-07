//
// Created by sr-71 on 04/02/2024.
//

#include "rclone.hpp"
#include <iostream>


namespace Iridium
{
    namespace ba = boost::asio;
    namespace bp = boost::process;
    namespace bs2 = boost::signals2;
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
        _signal_every_line = std::make_unique<bs2::signal<void(const std::string&)>>();
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
        while (std::getline(*_out, line) && !_out->eof())
        {
            if (_signal_every_line != nullptr)
                (*_signal_every_line)(line);
//            std ::cout << boost::this_thread::get_id() << std::endl;
        }
        _out.reset();
    }

    void rclone::read_error()
    {
        std::string line;
        while (std::getline(*_err, line) && !_err->eof())
            std::cerr << line << std::endl;
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
                    _path_rclone, _args,
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
        { read_output(); });
        ba::post(_pool, [this]
        { read_error(); });

        ba::post(_pool, [this]
        {
            _child.wait();
            _state = state::finished;
            if (_signal_finish != nullptr)
                (*_signal_finish)(_child.exit_code());
            _in.reset();
            _cv.notify_all();

        });

        return *this;

    }

    rclone &rclone::version()
    {
        _args.emplace_back("--version");
        return *this;
    }

    void rclone::stop()
    {
        _in.reset();
        _pool.stop();
        _child.terminate();
        _state = state::stopped;
    }

    rclone &rclone::config()
    {
        _args.emplace_back("config");
        return *this;
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

    rclone &rclone::every_line(const std::function<void(const std::string&)> &&callback)
    {

        _signal_every_line->connect(
                [this,callback](const std::string &line)
                {
                    ba::post(_pool, [callback, line]
                    {
                        callback(line);
                    });
                }
        );
        return *this;
    }

    rclone &rclone::finished(const std::function<void(int)> &&callback)
    {
        _signal_finish->connect(
                [this,callback](const int &exit_code)
                {
                    ba::post(_pool, [callback, exit_code]
                    {
                        callback(exit_code);
                    });
                }
        );
        return *this;
    }

} // namespace Iridium