#include <basic_option.hpp>
#include <boost/algorithm/string/join.hpp>


namespace iridium::rclone::option
{
	void basic_option::add_option_to_vector(const basic_option& option, std::vector<std::string>& vector)
	{
		vector.insert(vector.end(), option._options.begin(), option._options.end());
	}

	void basic_option::add_options_to_vector(const option::vector& options, std::vector<std::string>& vector)
	{
		for (const auto& opt: options)
			add_option_to_vector(opt, vector);
	}


	auto operator<<(std::ostream & os, const basic_option & obj) -> std::ostream &
	{
		return os << boost::algorithm::join(obj._options, " ");
	}
}; // namespace iridium::rclone::option
