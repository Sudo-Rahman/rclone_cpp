#define BOOST_TEST_MODULE RcloneFileTest
#include <boost/test/included/unit_test.hpp>
#include <boost/test/utils/lazy_ostream.hpp>
#include <iridium/entities.hpp>

using namespace iridium::rclone;
using namespace entities;
using namespace std;
using namespace std::chrono;

BOOST_AUTO_TEST_SUITE(Suite)

	BOOST_AUTO_TEST_CASE(testRcloneFileEquality)
	{
		auto r_ptr = remote::create_shared_ptr("TestRemote", remote::google_drive,
		                                       "/tests/path");
		auto r_ptr2 = remote::create_shared_ptr("AnotherRemote", remote::sftp, "/another/path");

		auto clock = system_clock::now();

		file file1(nullptr, "path", 100, false, clock, r_ptr);
		file file2(nullptr, "path", 100, false, clock, r_ptr);
		file file3(nullptr, "path2", 100, false, clock, r_ptr2);

		BOOST_CHECK(file1 == file2);
		BOOST_CHECK(file1 != file3);
	}

	BOOST_AUTO_TEST_CASE(testRcloneFileOutputOperator)
	{
		auto r_ptr = remote::create_shared_ptr("TestRemote", remote::google_drive,
		                                       "/tests/path");

		file file(nullptr, "path", 100, false, system_clock::now(), r_ptr);

		ostringstream oss;
		oss << file;

		BOOST_CHECK(!oss.str().empty());
	}

	BOOST_AUTO_TEST_CASE(testRcloneFileSetters)
	{
		auto r_ptr = remote::create_shared_ptr("TestRemote", remote::google_drive,
		                                       "/tests/path");

		auto clock = system_clock::now();
		file file(nullptr, "path", 100, false, system_clock::now(), r_ptr);

		file.set_name("path");
		file.set_remote(remote::create_shared_ptr("NewRemote", remote::sftp, "/new/path"));
		file.set_size(200);
		file.set_is_dir(true);
		file.set_mod_time(clock);

		BOOST_CHECK_EQUAL(file.name(), "path");
		BOOST_CHECK_EQUAL(file.remote()->name(), "NewRemote");
		BOOST_CHECK_EQUAL(file.size(), 200);
		BOOST_CHECK_EQUAL(file.is_dir(), true);
		BOOST_CHECK_EQUAL(file.mod_time().time_since_epoch().count(), clock.time_since_epoch().count());
	}

	BOOST_AUTO_TEST_CASE(testRcloneFileCopyConstructor)
	{
		auto r_ptr = remote::create_shared_ptr("TestRemote", remote::google_drive, "/tests/path");

		file f(nullptr, "path", 100, false, system_clock::now(), r_ptr);
		file fileCopy(f);

		BOOST_CHECK(f == fileCopy);
	}

	BOOST_AUTO_TEST_CASE(testRcloneFileMoveConstructor)
	{
		auto r_ptr = remote::create_shared_ptr("TestRemote", remote::google_drive, "/tests/path");

		auto clock = system_clock::now();
		file f(nullptr, "path", 100, false, clock, r_ptr);
		file fileCopy(std::move(f));

		BOOST_CHECK_EQUAL(fileCopy.name(), "path");
		BOOST_CHECK_EQUAL(fileCopy.remote()->name(), "TestRemote");
		BOOST_CHECK_EQUAL(fileCopy.size(), 100);
		BOOST_CHECK_EQUAL(fileCopy.is_dir(), false);
		BOOST_CHECK_EQUAL(fileCopy.mod_time().time_since_epoch().count(), clock.time_since_epoch().count());
	}

	BOOST_AUTO_TEST_CASE(testRcloneFileCopyAssignment)
	{
		auto r_ptr = remote::create_shared_ptr("TestRemote", remote::google_drive, "/tests/path");

		file f(nullptr, "path", 100, false, system_clock::now(), r_ptr);
		file fileCopy = f;

		BOOST_CHECK(f == fileCopy);
	}

	BOOST_AUTO_TEST_CASE(testRcloneFileMoveAssignment)
	{
		auto r_ptr = remote::create_shared_ptr("TestRemote", remote::google_drive, "/tests/path");

		auto clock = system_clock::now();
		file f(nullptr, "path", 100, false, clock, r_ptr);
		file fileCopy = std::move(f);

		BOOST_CHECK_EQUAL(fileCopy.name(), "path");
		BOOST_CHECK_EQUAL(fileCopy.remote()->name(), "TestRemote");
		BOOST_CHECK_EQUAL(fileCopy.size(), 100);
		BOOST_CHECK_EQUAL(fileCopy.is_dir(), false);
		BOOST_CHECK_EQUAL(fileCopy.mod_time().time_since_epoch().count(), clock.time_since_epoch().count());
	}

	BOOST_AUTO_TEST_CASE(testRcloneFileMoveAssignmentSetter)
	{
		auto r_ptr = remote::create_shared_ptr("TestRemote", remote::google_drive, "/tests/path");

		auto clock = system_clock::now();
		file f(nullptr, "path", 100, false, clock, r_ptr);
		file fileCopy = std::move(f);

		BOOST_CHECK_EQUAL(fileCopy.name(), "path");
		BOOST_CHECK_EQUAL(fileCopy.remote()->name(), "TestRemote");
		BOOST_CHECK_EQUAL(fileCopy.size(), 100);
		BOOST_CHECK_EQUAL(fileCopy.is_dir(), false);
		BOOST_CHECK_EQUAL(fileCopy.mod_time().time_since_epoch().count(), clock.time_since_epoch().count());

		clock = system_clock::now();
		fileCopy.set_name("new");
		fileCopy.set_remote(remote::create_shared_ptr("NewRemote", remote::sftp, "/new/path"));
		fileCopy.set_size(200);
		fileCopy.set_is_dir(true);
		fileCopy.set_mod_time(clock);

		BOOST_CHECK_EQUAL(fileCopy.name(), "new");
		BOOST_CHECK_EQUAL(fileCopy.remote()->name(), "NewRemote");
		BOOST_CHECK_EQUAL(fileCopy.size(), 200);
		BOOST_CHECK_EQUAL(fileCopy.is_dir(), true);
		BOOST_CHECK_EQUAL(fileCopy.mod_time().time_since_epoch().count(), clock.time_since_epoch().count());
	}

	BOOST_AUTO_TEST_CASE(testRcloneFileAddChild)
	{
		auto r_ptr = remote::create_shared_ptr("TestRemote", remote::google_drive, "/tests/path");

		file file(nullptr, "parent", 100, true, system_clock::now(), r_ptr);
		file.add_child(file::create_shared_ptr(&file, "child", 100, false,
		                                       system_clock::now(), r_ptr));

		BOOST_CHECK_EQUAL(file.nb_chilchren(), 1);
		BOOST_CHECK_EQUAL(file.parent(), nullptr);
		BOOST_CHECK_EQUAL(file.children().at(0)->parent(), &file);
		BOOST_CHECK_EQUAL(file.children().at(0)->name(), "child");
		BOOST_CHECK_EQUAL(file.children().at(0)->path(), "/parent/child");
	}

	BOOST_AUTO_TEST_CASE(testRcloneFileAbsolutePath)
	{
		auto r_ptr = remote::create_shared_ptr("TestRemote", remote::google_drive, "");
		file _file_(nullptr, "", 100, true, system_clock::now(), r_ptr);

		file child(&_file_, "parent.jpeg", 100, false, system_clock::now(), r_ptr);

		BOOST_CHECK_EQUAL(_file_.absolute_path(), "TestRemote:");
		BOOST_CHECK_EQUAL(child.absolute_path(), "TestRemote:parent.jpeg");
	}

	BOOST_AUTO_TEST_CASE(testRcloneFilePath)
	{
		auto r_ptr = remote::create_shared_ptr("TestRemote", remote::google_drive, "/tests/path");
		file file(nullptr, "path", 100, false, system_clock::now(), r_ptr);

		BOOST_CHECK_EQUAL(file.path(), "/path");
	}

	BOOST_AUTO_TEST_CASE(testRcloneFileParentDir)
	{
		auto r_ptr = remote::create_shared_ptr("TestRemote", remote::google_drive, "/tests/path");
		file parent(nullptr, "parent", 100, true, system_clock::now(), r_ptr);
		file child(&parent, "child", 100, false, system_clock::now(), r_ptr);

		BOOST_CHECK_EQUAL(child.parent_dir(), "/parent");
		BOOST_CHECK_EQUAL(child.parent_absolute_dir(), "TestRemote:/tests/path/parent");
	}

	BOOST_AUTO_TEST_CASE(testRcloneFileAddChildIfNotExist)
	{
		auto r_ptr = remote::create_shared_ptr("TestRemote", remote::google_drive, "/tests/path");

		auto clock = system_clock::now();
		file file(nullptr, "parent", 100, true, system_clock::now(), r_ptr);
		file.add_child_if_not_exist(file::create_shared_ptr(&file, "child", 100, false,
		                                                    clock, r_ptr));

		BOOST_CHECK_EQUAL(file.nb_chilchren(), 1);
		BOOST_CHECK_EQUAL(file.children().at(0)->parent(), &file);
		BOOST_CHECK_EQUAL(file.children().at(0)->name(), "child");
		BOOST_CHECK_EQUAL(file.children().at(0)->path(), "/parent/child");

		file.add_child_if_not_exist(file::create_shared_ptr(&file, "child", 100, false,
		                                                    clock, r_ptr));

		BOOST_CHECK_EQUAL(file.nb_chilchren(), 1); // child already exists, no new child added
	}

	BOOST_AUTO_TEST_CASE(testRcloneFileRemoveChild)
	{
		auto r_ptr = remote::create_shared_ptr("TestRemote", remote::google_drive, "/tests/path");

		auto clock = system_clock::now();
		file file(nullptr, "parent", 100, true, system_clock::now(), r_ptr);
		file.add_child_if_not_exist(file::create_shared_ptr(&file, "child", 100, false,
		                                                    clock, r_ptr));

		BOOST_CHECK_EQUAL(file.nb_chilchren(), 1);

		file.remove_child(file.children().at(0));

		BOOST_CHECK_EQUAL(file.nb_chilchren(), 0);
	}

BOOST_AUTO_TEST_SUITE_END()
