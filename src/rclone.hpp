//
// Created by sr-71 on 04/02/2024.
//

#ifndef RCLONE_RCLONE_HPP
#define RCLONE_RCLONE_HPP

#include <string>
#include <boost/process.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <future>
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

        rclone &version();

        rclone &config();

        void stop();

        static const std::string endl;
        rclone &operator<<(const std::string &input);

        void write_input(const std::string &input);

        rclone &every_line(const std::function<void(const std::string&)> &&callback);

        rclone &finished(const std::function<void(int)> &&callback);


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

        boost::asio::thread_pool _pool{6};

        std::vector<std::string> _args;

        void read_output();

        void read_error();

        std::unique_ptr<boost::signals2::signal<void(const std::string &line)>> _signal_every_line{};
        std::unique_ptr<boost::signals2::signal<void(int)>> _signal_finish{};

    };
} // namespace Iridium


#endif //RCLONE_RCLONE_HPP
