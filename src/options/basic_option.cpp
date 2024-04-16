#include <basic_option.hpp>
#include <iostream>

namespace iridium::rclone::option
{
	auto basic_option::get() -> std::vector<std::string>
	{
		if (_value.has_value())
			return {_option, _value.value()};
		return {_option};
	}

	basic_option::basic_option(const std::string &option) { _option = option; }

	basic_option::basic_option(const std::string &option, const std::string &value) : basic_option(option)
	{
		_value = value;
	}

	auto basic_option::uptr(const std::string &option) -> basic_opt_uptr
	{
		return std::make_unique<basic_option>(option);
	}

	auto basic_option::uptr(const std::string &option, const std::string &value) -> basic_opt_uptr
	{
		return std::make_unique<basic_option>(option, value);
	}

	void basic_option::add_option_to_vector(const basic_opt_uptr &option, std::vector<std::string> &vector)
	{
		std::vector<std::string> vec = option->get();
		std::erase(vec, "");
		vector.insert(vector.end(), vec.begin(), vec.end());
	}

	void basic_option::add_options_to_vector(const std::vector<basic_opt_uptr> &options,
	                                         std::vector<std::string> &vector)
	{
		for (const auto &opt: options)
			add_option_to_vector(opt, vector);
	}

	auto operator<<(std::ostream &os, const basic_option &obj) -> std::ostream&
	{
		return os << obj._option << " " << (obj._value.has_value() ? obj._value.value() : "null");
	}
}; // namespace iridium::rclone::option
