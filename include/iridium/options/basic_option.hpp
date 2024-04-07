#pragma once

#include <optional>
#include <ostream>
#include <string>
#include <memory>
#include <vector>

namespace iridium::rclone::option
{
	class basic_option;
	using uptr_basic_opt = std::unique_ptr<basic_option>;
	using ptr_basic_opt = std::shared_ptr<basic_option>;

	class basic_option
	{
	public:
		[[nodiscard]] virtual auto get() -> std::vector<std::string>;

		basic_option() = default;

		explicit basic_option(const std::string& option);

		basic_option(const std::string& option, const std::string& value);

		virtual ~basic_option() = default;

		basic_option(const basic_option&) = default;

		basic_option(basic_option&&) = default;

		auto operator=(const basic_option&) -> basic_option& = default;

		auto operator=(basic_option&&) -> basic_option& = default;

		[[nodiscard]] auto option() const -> std::string { return _option; }

		[[nodiscard]] auto value() const -> std::optional<std::string> { return _value; }

	private:
		std::string _option;
		std::optional<std::string> _value;

	public:
		static void add_option_to_vector(const uptr_basic_opt& option, std::vector<std::string>& vector);

		static void add_options_to_vector(const std::vector<uptr_basic_opt>& options, std::vector<std::string>& vector);

		friend auto operator<<(std::ostream& os, const basic_option& obj) -> std::ostream&;
	};

	using uptr_basic_opt = std::unique_ptr<basic_option>;
	using ptr_basic_opt = std::shared_ptr<basic_option>;
}
