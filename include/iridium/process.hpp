#pragma once

#include <string>
#include <boost/process.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <future>
#include "file.hpp"
#include "entities.hpp"
#include <boost/signals2.hpp>

namespace Iridium::rclone
{
    class process {

    public:

        process();

        ~process();

        enum class state {
            not_launched,
            launched,
            stopped,
            error,
            finished
        };

        static void initialize(const std::string &path_rclone);

        process &wait_for_start();

        process &wait_for_finish();

        process &execute();

        [[nodiscard]] int exit_code() const;

        [[nodiscard]] state get_state() const { return _state; }

        [[nodiscard]] std::vector<std::string> get_output() const { return _output; }

        [[nodiscard]] std::vector<std::string> get_error() const { return _error; }

        void stop();

        static const std::string endl;
        process &operator<<(const std::string &input);

        void write_input(const std::string &input);

        process &every_line(const std::function<void(const std::string&)> &&callback);

        process &finished(const std::function<void(int)> &&callback);

        process &version();

        process &list_remotes(std::vector<remote> &remotes);

        process &delete_remote(const remote &remote);

        process &config();

        process &lsjson(const remote &remote);

        process &lsjson(file &file);

        process &lsjson(file &file, const std::function<void(Iridium::rclone::file)> &&callback);

        process &copy_to(const file &source, const file &destination);

        process &move_to(const file &source, const file &destination);

        process &delete_file(const file &file);

        process &mkdir(const file &file);

        process &cat(const file &file);

        process &about(const remote &remote, std::function<void(const Iridium::rclone::about &)> &&callback);

        process &size(const file &file, std::function<void(const Iridium::rclone::size &)> &&callback);


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
} // namespace Iridium::rclone

