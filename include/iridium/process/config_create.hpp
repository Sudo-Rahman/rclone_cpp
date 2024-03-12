#pragma once
#include <string>

namespace iridium::rclone
{
	class process;

	class config_create
	{
		process * _process;
		std::string _name;
		std::string _type;

		auto check_attributes() const -> void;

	public:
		explicit config_create(process * process);

		[[nodiscard]] auto process() const -> rclone::process&;

		[[nodiscard]] auto execute() const -> rclone::process&;

		auto name(const std::string& name) -> config_create&;

		auto type(const std::string& type) -> config_create&;
	};
} // namespace iridium::rclone
