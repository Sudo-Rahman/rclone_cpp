#pragma once

#include <string>
#include <boost/process.hpp>
#include <boost/asio.hpp>
#include <functional>
#include "../entities.hpp"
#include "../options.hpp"
#include "../parsers/basic_parser.hpp"
#include "../parsers/file_parser.hpp"
#include <boost/signals2.hpp>

namespace iridium::rclone
{
    class process
    {
    public:
        process();

        ~process();

        enum class state
        {
            not_launched,
            launched,
            stopped,
            error,
            finished
        };

        /**
         * @brief Initialize the rclone process
         * @param path_rclone the path to the rclone executable or if empty the system path will be used
         */
        static void initialize(const std::string& path_rclone = "");

        process& wait_for_start();

        process& wait_for_finish();

        auto close_input_pipe() -> process&;

        auto close_output_pipe() -> process&;

        auto close_error_pipe() -> process&;

        process& execute();

        [[nodiscard]] int exit_code() const;

        [[nodiscard]] state get_state() const { return _state; }

        [[nodiscard]] std::vector<std::string> get_output() const { return _output; }

        [[nodiscard]] std::vector<std::string> get_error() const { return _error; }

        [[nodiscard]] option::vector get_options() const { return _local_options; }

        [[nodiscard]] static option::vector get_global_options() { return _global_options; }

        void stop();

        static const std::string endl;

        process& operator<<(const std::string& input);

        void write_input(const std::string& input);

        process& every_line(std::function<void(const std::string&)>&& callback);

        template<class T>
        auto every_line_parser(parser::basic_parser<T>&& parser) -> process&
        {
            _signal_every_line->connect([this, &parser](const std::string& line) { parser.parse(line); });
            return *this;
        }

        template<class T>
        auto every_line_parser(parser::basic_parser<T>& parser) -> process&
        {
            _signal_every_line->connect([this, &parser](const std::string& line) { parser.parse(line); });
            return *this;
        }

        process& finished(std::function<void(int)>&& callback);

        template<class T>
        process& finished_parser(parser::basic_parser<T>& parser)
        {
            _signal_finish->connect([this, &parser](int code) { parser.parse(boost::algorithm::join(_output, endl)); });
            return *this;
        }

        template<class T>
        process& finished_parser(parser::basic_parser<T>&& parser)
        {
            _signal_finish->connect([this, &parser](int code) { parser.parse(boost::algorithm::join(_output, endl)); });
            return *this;
        }

        process& finished_error(std::function<void()>&& callback);

        process& version();

        process& list_remotes(std::function<void(const std::vector<remote_ptr> &)> &&);

        process& list_remotes();

        process& delete_remote(const entitie::remote& remote);

        process& config();

        process& lsjson(const entitie::remote& remote);

        process& lsjson(entitie::file& file);

        process& copy_to(const entitie::file& source, const entitie::file& destination);

        process& move_to(const entitie::file& source, const entitie::file& destination);

        process& delete_file(const entitie::file& file);

        process& mkdir(const entitie::file& file);

        process& cat(const entitie::file& file);

        process& about(const entitie::remote& remote);

        process& size(const entitie::file& file);

        process& tree(const entitie::file& file);

        process& add_option(const option& option);

        template<class... Args>
        process& add_option(const option& option1, Args&&... args)
        {
            add_option(option1);
            add_option(std::forward<Args>(args)...);
            return *this;
        }

        static void add_global_option(const option& option);

        template<class... Args>
        static void add_global_option(const option& option1, Args&&... args)
        {
            add_global_option(option1);
            add_global_option(std::forward<Args>(args)...);
        }

    private:
        static std::string _path_rclone;
        static bool _is_initialized;
        static option::vector _global_options;

        std::mutex _mutex{};
        std::condition_variable _cv{};

        state _state{state::not_launched};

        boost::process::child _child{};
        std::unique_ptr<boost::process::ipstream> _out{};
        std::unique_ptr<boost::process::ipstream> _err{};
        std::unique_ptr<boost::process::opstream> _in{};

        boost::asio::thread_pool _pool{5};

        std::vector<std::string> _args;
        std::vector<std::string> _output{};
        std::vector<std::string> _error{};

        option::vector _local_options{};

        std::vector<parser::basic_parser<entitie>> _parsers{};

        void read_output();

        void read_error();

        std::unique_ptr<boost::signals2::signal<void(const std::string& line)>> _signal_every_line{};
        std::unique_ptr<boost::signals2::signal<void(int)>> _signal_finish{};
    };
} // namespace iridium::rclone
