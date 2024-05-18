#include "process/config_create.hpp"
#include <string>
#include <vector>
#include "process_impl.cpp"

using namespace std;

namespace iridium::rclone
{
	auto config_create::check_attributes() const -> void
	{
		if (_name.empty())
			throw std::runtime_error("name must be set");
		if (_type.empty())
			throw std::runtime_error("type must be set");
	}

	config_create::config_create(process *process) : _process(process) {}

	auto config_create::get_process() const -> process&
	{
		check_attributes();
		_process->_impl->_args = {"config", "create", _name, _type};
		_process->_impl->_args.insert(_process->_impl->_args.end(), _params.begin(), _params.end());
		return *_process;
	}

	auto config_create::execute() const -> rclone::process&
	{
		check_attributes();
		_process->_impl->_args = {"config", "create", _name, _type};
		_process->_impl->_args.insert(_process->_impl->_args.end(), _params.begin(), _params.end());
		return _process->execute();
	}

	auto config_create::name(const std::string &name) -> config_create&
	{
		_name = name;
		return *this;
	}

	auto config_create::type(const std::string &type) -> config_create&
	{
		_type = type;
		return *this;
	}

	auto config_create::add_param(const std::string &param) -> config_create&
	{
		_params.push_back(param);
		return *this;
	}
} // namespace iridium::rclone
