#pragma once
#include <string>
#include <vector>

namespace iridium::rclone
{
	class process;

	class config_create
	{
		process * _process;
		std::string _name;
		std::string _type;
		std::vector<std::string> _params;

		auto check_attributes() const -> void;

	public:
		explicit config_create(process * process);

		[[nodiscard]] auto get_process() const -> process&;

		auto execute() const -> process&;

		auto name(const std::string& name) -> config_create&;

		auto type(const std::string& type) -> config_create&;

		auto add_param(const std::string& param) -> config_create&;

		template<class... Args>
		auto add_param(const std::string& param, Args... args) -> config_create&
		{
			_params.push_back(param);
			add_param(args...);
			return *this;
		}
	};
} // namespace iridium::rclone
