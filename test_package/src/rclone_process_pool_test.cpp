#include <vector>
#define BOOST_TEST_MODULE RcloneProcessPoolTest
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/included/unit_test.hpp>
#include <iridium/rclone.hpp>
#include <memory>
#include <atomic>

using namespace iridium::rclone;
using namespace std;

auto create_pool() -> process_pool
{
	process::initialize();
	return process_pool(5);
}

BOOST_AUTO_TEST_SUITE(Suite)

	BOOST_AUTO_TEST_CASE(ProcessPoolInitializationTest)
	{
		auto pool = create_pool();
		BOOST_CHECK_EQUAL(pool.size(), 0); // The pool should be empty initially
		BOOST_CHECK(pool.empty()); // The pool should be empty initially
	}

	BOOST_AUTO_TEST_CASE(ProcessPoolAddProcessTest)
	{
		auto pool = create_pool();
		auto p = std::make_unique<process>();
		p->version();
		pool.add_process(std::move(p));
		BOOST_CHECK_EQUAL(pool.size(), 1); // The pool should have 1 process
		auto p2 = std::make_unique<process>();
		p2->version();
		pool.add_process(std::move(p2));
		BOOST_CHECK_EQUAL(pool.size(), 2); // The pool should have 2 processes
		BOOST_CHECK(!pool.empty()); // The pool should not be empty
	}

	BOOST_AUTO_TEST_CASE(ProcessPoolClearPoolTest)
	{
		auto pool = create_pool();
		auto p = std::make_unique<process>();
		p->version();
		pool.add_process(std::move(p));
		pool.clear_pool();
		BOOST_CHECK_EQUAL(pool.size(), 0); // The pool should be empty after clearing
		BOOST_CHECK(pool.empty()); // The pool should be empty after clearing
	}

	BOOST_AUTO_TEST_CASE(ProcessPoolSetSimultaneousProcessesTest)
	{
		auto pool = create_pool();
		pool.set_simultaneous_processes(10);
		BOOST_CHECK_EQUAL(pool.simultaneous_processes(), 10);
	}

	BOOST_AUTO_TEST_CASE(ProcessPoolWaitTest)
	{
		auto pool = create_pool();

		auto counter = std::atomic<uint8_t>(0);
		for (int i = 0; i < 10; ++i)
		{
			auto proc = std::make_unique<process>();
			proc->list_remotes()
					.on_finish([&](int) { counter++; });
			pool.add_process(std::move(proc), process_pool::priority::low);
		}
		pool.wait();

		BOOST_CHECK_EQUAL(counter, 10);
	}

	BOOST_AUTO_TEST_CASE(ProcessPoolStopTest)
	{
		auto pool = create_pool();
		pool.set_simultaneous_processes(1);
		for (int i = 0; i < 100; i++)
		{
			auto p = std::make_unique<process>();
			p->list_remotes();
			pool.add_process(std::move(p));
		}
		pool.stop();
		BOOST_CHECK_EQUAL(pool.get_state(), process_pool::state::stopped);
	}

	BOOST_AUTO_TEST_CASE(ProcessPoolStopAllProcessesTest)
	{
		auto pool = create_pool();
		pool.set_simultaneous_processes(1);
		std::vector<process *> processes;
		for (int i = 0; i < 100; i++)
		{
			auto p = std::make_unique<process>();
			p->list_remotes();
			processes.push_back(p.get());
			pool.add_process(std::move(p));
		}
		pool.stop_all_processes();
		bool stopped = false;
		for (auto p: processes)
		{
			if (p->get_state() == process::stopped)
			{
				stopped = true;
				break;
			}
		}
		BOOST_CHECK(stopped);
	}

	BOOST_AUTO_TEST_CASE(ProcessPoolStopAllProcessesAndClearTest)
	{
		auto pool = create_pool();
		auto p1 = std::make_unique<process>();
		p1->version();
		auto p2 = std::make_unique<process>();
		p2->version();
		pool.add_process(std::move(p1));
		pool.add_process(std::move(p2));
		pool.stop_all_processes();
		pool.clear_pool();
		BOOST_CHECK_EQUAL(pool.size(), 0)
		; // The pool should be empty after stopping all processes and clearing the pool
		BOOST_CHECK(pool.empty()); // The pool should be empty after stopping all processes and clearing the pool
	}

BOOST_AUTO_TEST_SUITE_END()
