#define BOOST_TEST_MODULE RcloneProcessTest
#include <boost/dll/runtime_symbol_info.hpp>
#include <vector>
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/included/unit_test.hpp>
#include <boost/test/utils/lazy_ostream.hpp>
#include <iridium/rclone.hpp>
#include <utility>
#include <boost/dll.hpp>
#include <boost/filesystem/path.hpp>

using namespace iridium::rclone;
using namespace entities;
using namespace std;
using namespace std::chrono;

struct ProcessFixture
{
	ProcessFixture()
	{
		// Initialisation du process
		process::initialize();
	}
};

BOOST_FIXTURE_TEST_SUITE(ProcessTestsSuite, ProcessFixture)

	BOOST_AUTO_TEST_CASE(ProcessInitializationTest)
	{
		BOOST_CHECK(process::initialize()); // Vérifie si l'initialisation a réussi
	}

	BOOST_AUTO_TEST_CASE(ProcessExecutionTest)
	{
		process p;
		p.version().execute(); // Exemple d'exécution d'une commande
		BOOST_CHECK(p.is_running()); // Vérifie si le processus est en cours d'exécution
		p.wait_for_finish();
		BOOST_CHECK(!p.is_running()); // Vérifie si le processus s'est terminé
		BOOST_CHECK_EQUAL(p.exit_code(), 0); // Vérifie le code de sortie du processus
	}

	BOOST_AUTO_TEST_CASE(ProcessGlobalOptionsTest)
	{
		process p;
		p.with_global_options();
		auto opt = option::logging::use_json_log();
		auto pointer = opt.get();
		process::add_global_option(std::move(opt));
		auto val = std::ranges::any_of(process::global_options(), [&](const auto &o)
		{
			return o->option() == pointer->option();
		});
		BOOST_CHECK(val == true);
	}

	BOOST_AUTO_TEST_CASE(ProcessOptionsTest)
	{
		process p;
		auto opt = option::logging::use_json_log();
		auto pointer = opt.get();
		p.add_option(std::move(opt));
		auto val = std::ranges::any_of(process::global_options(), [&](const auto &o)
		{
			return o->option() == pointer->option();
		});
		BOOST_CHECK(val == true);
	}

	BOOST_AUTO_TEST_CASE(ProcessOutputTest)
	{
		process p;
		p.version().execute();
		p.wait_for_finish();
		auto output = p.get_output();
		BOOST_CHECK(!output.empty());
	}

	BOOST_AUTO_TEST_CASE(ProcessErrorTest)
	{
		process p;
		p.version().execute();
		p.wait_for_finish();
		auto error = p.get_error();
		BOOST_CHECK(error.empty());
	}

	BOOST_AUTO_TEST_CASE(ProcessCommandsTest)
	{
		process p;
		p.version().execute();
		p.wait_for_finish();
		auto commands = p.commands();
		BOOST_CHECK(!commands.empty());
	}

	BOOST_AUTO_TEST_CASE(ProcessStopTestException)
	{
		process p;
		p.version().execute().wait_for_finish();
		BOOST_CHECK_THROW(p.stop(), std::runtime_error);
	}

	BOOST_AUTO_TEST_CASE(ProcessStopTest)
	{
		process p;
		p.version().execute().wait_for_finish();
		BOOST_CHECK(!p.is_running());
	}

	BOOST_AUTO_TEST_CASE(ProcessCheckAndWaitForStartTest)
	{
		process p;
		auto src = file(nullptr, "", 0, true, system_clock::now(),
		                remote::create_shared_ptr("", remote::google_drive, "/"));
		auto dst = src;
		p.check(src, dst).execute();
		p.wait_for_start();
		BOOST_CHECK(p.is_running());
		p.stop();
		BOOST_CHECK(!p.is_running());
	}

	BOOST_AUTO_TEST_CASE(config_createTest)
	{
		process().delete_remote(remote("test", remote::ftp, "")).execute().wait_for_finish();
		auto p = process();
		p.config_create().name("test")
				.type(remote::remote_type_to_string(remote::ftp))
				.add_param("host=ftp.dlptest.com",
				           "user=dlpuser",
				           "pass=rNrKYTX9g7z3RgJRmxWuGHbeu")
				.execute().wait_for_finish();
		BOOST_CHECK_EQUAL(p.exit_code(), 0);
		bool found = false;
		process().list_remotes([&found](const std::vector<remote_ptr> &remotes)
		{
			found = std::ranges::any_of(remotes, [](const auto &r)
			{
				return r->name() == "test" and r->type() == remote::ftp;
			});
		}).execute().wait_for_finish();
		BOOST_CHECK(found == true);
	}

	const file test_file = file(nullptr, "/", 0, true, system_clock::now(),
	                            remote::create_shared_ptr("test", remote::ftp, ""));

	BOOST_AUTO_TEST_CASE(lsjsonTest)
	{
		auto p = process();
		p.lsjson(test_file)
				.execute().wait_for_finish();
		BOOST_CHECK_EQUAL(p.exit_code(), 0);
	}

	BOOST_AUTO_TEST_CASE(LsjsonRemoteTest)
	{
		process p;
		p.lsjson(*test_file.remote()).execute().wait_for_finish();
		BOOST_CHECK(p.exit_code() == 0);
	}

	const auto test_file_exception = file(nullptr, "/", 0, false, system_clock::now(),
	                                      remote::create_shared_ptr("", remote::none, ""));

	BOOST_AUTO_TEST_CASE(lsjsonExceptionTest)
	{
		auto p = process();
		BOOST_CHECK_THROW(p.lsjson(test_file_exception), std::runtime_error);
	}

	BOOST_AUTO_TEST_CASE(lsTest)
	{
		auto p = process();
		p.ls(test_file).execute().wait_for_finish();
		BOOST_CHECK_EQUAL(p.exit_code(), 0);
	}

	BOOST_AUTO_TEST_CASE(lsExceptionTest)
	{
		auto p = process();
		BOOST_CHECK_THROW(p.ls(test_file_exception), std::runtime_error);
	}

	BOOST_AUTO_TEST_CASE(lslTest)
	{
		auto p = process();
		p.lsl(test_file).execute().wait_for_finish();
		BOOST_CHECK_EQUAL(p.exit_code(), 0);
	}

	BOOST_AUTO_TEST_CASE(lslExceptionTest)
	{
		auto p = process();
		BOOST_CHECK_THROW(p.lsl(test_file_exception), std::runtime_error);
	}

	BOOST_AUTO_TEST_CASE(lsdTest)
	{
		auto p = process();
		p.lsd(test_file).execute().wait_for_finish();
		BOOST_CHECK_EQUAL(p.exit_code(), 0);
	}

	BOOST_AUTO_TEST_CASE(lsdExceptionTest)
	{
		auto p = process();
		BOOST_CHECK_THROW(p.lsd(test_file_exception), std::runtime_error);
	}

	BOOST_AUTO_TEST_CASE(lsfTest)
	{
		auto p = process();
		p.lsf(test_file).execute().wait_for_finish();
		BOOST_CHECK_EQUAL(p.exit_code(), 0);
	}

	BOOST_AUTO_TEST_CASE(lsfExceptionTest)
	{
		auto p = process();
		BOOST_CHECK_THROW(p.lsf(test_file_exception), std::runtime_error);
	}

	const auto test_file_path = boost::dll::program_location().parent_path().append("test.txt");
	const auto programme_file_path = boost::dll::program_location();
	const auto programme_dir_path = boost::dll::program_location().parent_path();
	const auto programme_dir_test_path = boost::dll::program_location().parent_path().append("test_folder");
	const auto none_remote = remote::create_shared_ptr("", remote::none, "");

	BOOST_AUTO_TEST_CASE(touchTest)
	{
		auto p = process();
		file touch(nullptr, test_file_path.string(), 0, false, system_clock::now(),
		           none_remote);
		p.touch(touch).execute().wait_for_finish();
		BOOST_CHECK_EQUAL(p.exit_code(), 0);
	}

	BOOST_AUTO_TEST_CASE(mkdirTest)
	{
		auto p = process();
		file f(nullptr, programme_dir_test_path.string(), 0, true, system_clock::now(),
		       none_remote);
		p.mkdir(f).execute().wait_for_finish();
		BOOST_CHECK_EQUAL(p.exit_code(), 0);
	}

	BOOST_AUTO_TEST_CASE(copyToTest)
	{
		auto p = process();
		file src(nullptr, test_file_path.string(), 0, false, system_clock::now(),
		         none_remote);
		auto dst_path = programme_dir_test_path;
		dst_path.append(test_file_path.filename());
		file dst(nullptr, dst_path.string(), 0, false, system_clock::now(),
		         none_remote);
		p.copy_to(src, dst).execute().wait_for_finish();
		BOOST_CHECK_EQUAL(p.exit_code(), 0);
	}

	BOOST_AUTO_TEST_CASE(deleteFileTest)
	{
		auto p = process();
		auto path = programme_dir_test_path;
		path.append(test_file_path.filename());
		file f(nullptr, path.string(), 0, false, system_clock::now(),
		       none_remote);
		p.delete_file(f).execute().wait_for_finish();
		BOOST_CHECK_EQUAL(p.exit_code(), 0);
	}

	BOOST_AUTO_TEST_CASE(moveToTest)
	{
		auto p = process();
		file src(nullptr, test_file_path.string(), 0, false, system_clock::now(),
		         none_remote);
		auto dst_path = programme_dir_test_path;
		dst_path.append(test_file_path.filename());
		file dst(nullptr, dst_path.string(), 0, true, system_clock::now(),
		         none_remote);
		p.move_to(src, dst).execute().wait_for_finish();
		BOOST_CHECK_EQUAL(p.exit_code(), 0);
	}

	BOOST_AUTO_TEST_CASE(rmdirTest)
	{
		auto dir = programme_dir_test_path;
		dir.append("rmdir");
		file f(nullptr, dir.string(), 0, true, system_clock::now(),
		       none_remote);
		process().mkdir(f).execute().wait_for_finish();
		auto p = process();
		p.rmdir(f).execute().wait_for_finish();
		BOOST_CHECK_EQUAL(p.exit_code(), 0);
	}

	BOOST_AUTO_TEST_CASE(rmdirsTest)
	{
		auto dir = programme_dir_test_path;
		dir.append("rmdir");
		file f(nullptr, dir.string(), 0, true, system_clock::now(),
		       none_remote);
		process().mkdir(f).execute().wait_for_finish();
		auto p = process();
		p.rmdirs(f).execute().wait_for_finish();
		BOOST_CHECK_EQUAL(p.exit_code(), 0);
	}

	BOOST_AUTO_TEST_CASE(purgeTest)
	{
		auto p = process();
		file f(nullptr, programme_dir_test_path.string(), 0, true, system_clock::now(),
		       none_remote);
		p.purge(f).execute().wait_for_finish();
		BOOST_CHECK_EQUAL(p.exit_code(), 0);
	}

	BOOST_AUTO_TEST_CASE(catTest)
	{
		auto p = process();
		file f(nullptr, programme_dir_path.string(), 0, true, system_clock::now(),
		       none_remote);
		p.cat(f).execute().wait_for_finish();
		BOOST_CHECK_EQUAL(p.exit_code(), 0);
		BOOST_CHECK(p.get_output().size() > 0);
	}

	BOOST_AUTO_TEST_CASE(aboutTest)
	{
		auto p = process();
		remote r("", remote::none, "/");
		p.about(r).execute().wait_for_finish();
		BOOST_CHECK_EQUAL(p.exit_code(), 0);
	}

	BOOST_AUTO_TEST_CASE(sizeTest)
	{
		auto p = process();
		file f(nullptr, programme_dir_path.string(), 0, true, system_clock::now(),
		       none_remote);
		p.size(f).execute().wait_for_finish();
		BOOST_CHECK_EQUAL(p.exit_code(), 0);
		BOOST_CHECK(p.get_output().size() > 0);
	}

	BOOST_AUTO_TEST_CASE(treeTest)
	{
		auto p = process();
		file f(nullptr, programme_dir_path.string(), 0, true, system_clock::now(),
		       none_remote);
		p.tree(f).execute().wait_for_finish();
		BOOST_CHECK_EQUAL(p.exit_code(), 0);
		BOOST_CHECK(p.get_output().size() > 0);
	}

BOOST_AUTO_TEST_SUITE_END()
