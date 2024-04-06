#pragma once

#include <ostream>
#include <string>
#include <vector>

namespace iridium::rclone::option
{
	class basic_option
	{
	public:
		[[nodiscard]] virtual auto get() -> std::vector<std::string> { return {_option, _value}; }

		basic_option() = default;

		explicit basic_option(const std::string& option);

		basic_option(const std::string& option, const std::string& value);

		virtual ~basic_option() = default;

		basic_option(const basic_option&) = default;

		basic_option(basic_option&&) = default;

		auto operator=(const basic_option&) -> basic_option& = default;

		auto operator=(basic_option&&) -> basic_option& = default;

		[[nodiscard]] auto option() const -> std::string { return _option; }

		[[nodiscard]] auto value() const -> std::string { return _value; }

	private:
		std::string _option;
		std::string _value;


		friend class process;

	public:
		static void add_option_to_vector(const basic_option& option, std::vector<std::string>& vector);

		static void add_options_to_vector(const std::vector<basic_option>& options, std::vector<std::string>& vector);

		friend auto operator<<(std::ostream& os, const basic_option& obj) -> std::ostream&;
	};

	using vector = std::vector<basic_option>;
}
