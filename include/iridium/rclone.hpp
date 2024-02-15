#pragma once

#include <string>
#include <boost/process.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <future>
#include "rclone_file.hpp"
#include <boost/signals2.hpp>

namespace Iridium
{
    class rclone {

    public:

        rclone();

        ~rclone();

        enum class state {
            not_launched,
            launched,
            stopped,
            error,
            finished
        };

        static void initialize(const std::string &path_rclone);

        rclone &wait_for_start();

        rclone &wait_for_finish();

        rclone &execute();

        [[nodiscard]] int exit_code() const;

        [[nodiscard]] state get_state() const { return _state; }

        [[nodiscard]] std::vector<std::string> get_output() const { return _output; }

        [[nodiscard]] std::vector<std::string> get_error() const { return _error; }

        void stop();

        static const std::string endl;
        rclone &operator<<(const std::string &input);

        void write_input(const std::string &input);

        rclone &every_line(const std::function<void(const std::string&)> &&callback);

        rclone &finished(const std::function<void(int)> &&callback);

        rclone &version();

        rclone &list_remotes(std::vector<rclone_remote> &remotes);

        rclone &delete_remote(const rclone_remote &remote);

        rclone &config();

        rclone &lsjson(const rclone_remote &remote);

        rclone &lsjson(rclone_file &file);

        rclone &lsjson(rclone_file &file, const std::function<void(rclone_file)> &&callback);

        rclone &copy_to(const rclone_file &source, const rclone_file &destination);

        rclone &move_to(const rclone_file &source, const rclone_file &destination);

        rclone &delete_file(const rclone_file &file);

        rclone &mkdir(const rclone_file &file);

        rclone &cat(const rclone_file &file);

        rclone &about(const rclone_remote &remote);

        rclone &size(const rclone_remote &remote);


    private:
        static std::string _path_rclone;
        static bool _is_initialized;

        std::mutex _mutex{};
        std::condition_variable _cv{};

        state _state{state::not_launched};

        boost::process::child _child{};
        std::unique_ptr<boost::process::ipstream> _out{};
        std::unique_ptr<boost::process::ipstream> _err{};
        std::unique_ptr<boost::process::opstream> _in{};

        boost::asio::thread_pool _pool{5};
        std::atomic_int_fast8_t _counter{0};

        std::vector<std::string> _args;
        std::vector<std::string> _output{};
        std::vector<std::string> _error{};

        void read_output();

        void read_error();

        std::unique_ptr<boost::signals2::signal<void(const std::string &line)>> _signal_every_line{};
        std::unique_ptr<boost::signals2::signal<void(int)>> _signal_finish{};

    };
} // namespace Iridium

