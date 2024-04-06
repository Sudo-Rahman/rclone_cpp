#include <basic_option.hpp>
#include <boost/algorithm/string/join.hpp>


namespace iridium::rclone::option
{
	basic_option::basic_option(const std::string & option)
	{
		_option = option;
	}

	basic_option::basic_option(const std::string & option, const std::string & value) : basic_option(option)
	{
		_value = value;
	}

	void basic_option::add_option_to_vector(const basic_option& option, std::vector<std::string>& vector)
	{
		vector.push_back(option._option);
		if (!option._value.empty())
			vector.push_back(option._value);
	}

	void basic_option::add_options_to_vector(const option::vector& options, std::vector<std::string>& vector)
	{
		for (const auto& opt: options)
			add_option_to_vector(opt, vector);
	}


	auto operator<<(std::ostream & os, const basic_option & obj) -> std::ostream &
	{
		return os << obj._option << " " << obj._value;
	}
}; // namespace iridium::rclone::option
