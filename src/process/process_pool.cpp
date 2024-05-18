#include "process_pool_impl.cpp"

namespace iridium::rclone
{
	process_pool::~process_pool()
	{
		delete _impl;
	}

	process_pool::process_pool(uint16_t simultaneous_processes)
	{
		_impl = new _process_pool_impl_(simultaneous_processes);
	}

	process_pool::process_pool(process_pool &&pool) noexcept
	{
		_impl = pool._impl;
		pool._impl = nullptr;
	}

	auto process_pool::operator=(process_pool &&pool) noexcept -> process_pool&
	{
		_impl = pool._impl;
		pool._impl = nullptr;
		return *this;
	}

	auto process_pool::add_process(std::unique_ptr<process> &&process, priority priority) const -> void
	{
		_impl->add_process(std::move(process), priority);
	}

	auto process_pool::add_process(process *process, priority priority) const -> void
	{
		add_process(process_uptr(process), priority);
	}

	void process_pool::stop() const { _impl->stop(); }

	void process_pool::stop_all_processes() const { _impl->stop_all_processes(); }

	void process_pool::wait() const { _impl->wait(); }

	auto process_pool::empty() const -> bool { return _impl->_processes.empty(); }

	auto process_pool::size() const -> uint16_t { return _impl->_processes.size(); }

	void process_pool::clear_pool() const { _impl->clear_pool(); }

	auto process_pool::set_simultaneous_processes(uint16_t simultaneous_processes) const -> void
	{
		_impl->set_simultaneous_processes(simultaneous_processes);
	}

	void process_pool::stop_all_processes_and_clear() const
	{
		stop_all_processes();
		clear_pool();
	}
}; // namespace iridium::rclone
