//
// Created by sr-71 on 04/02/2024.
//

#include "rclone.hpp"
#include <iostream>

namespace Iridium
{
    namespace ba = boost::asio;
    namespace bp = boost::process;
    std::string rclone::_path_rclone;
    bool rclone::_is_initialized = false;

    void rclone::initialize(const std::string &path_rclone)
    {
        _path_rclone = path_rclone;
        _is_initialized = true;
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
        *_in << input;
    }

    void rclone::read_output()
    {
        std::string line;
        while (std::getline(*_out, line) && !_out->eof())
        {
            std::cout << line << std::endl;
        }
        delete _out;
    }

    void rclone::read_error()
    {
        std::string line;
        while (std::getline(*_err, line) && !_err->eof())
        {
            std::cerr << line << std::endl;
        }
        delete _err;
    }

    rclone &rclone::execute()
    {
        if (!_is_initialized)
        {
            throw std::runtime_error("rclone not initialized");
        }

        if (_state != state::not_launched)
        {
            throw std::runtime_error("rclone already started");
        }

        try {
            _in = new bp::opstream{};
            _out = new bp::ipstream{};
            _err = new bp::ipstream{};
            _child = bp::child(
                    _path_rclone, _args,
                    bp::std_in < *_in, bp::std_out > *_out,
                    bp::std_err > *_err);
        }catch (const std::exception &e)
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
            delete _in;
            _cv.notify_all();
        });

        return *this;

    }

    rclone &rclone::version()
    {
        _args.emplace_back("--version");
        return *this;
    }


} // namespace Iridium