#define BOOST_TEST_MODULE RemoteTest

#include <boost/test/included/unit_test.hpp>
#include <boost/test/utils/lazy_ostream.hpp>
#include <iridium/entities.hpp>

using namespace iridium::rclone;
using namespace entities;
using namespace std;

BOOST_AUTO_TEST_SUITE(Suite)

	BOOST_AUTO_TEST_CASE(testRemoteEquality)
	{
		remote remote1("TestRemote", remote::google_drive, "/tests/path");
		remote remote2("TestRemote", remote::google_drive, "/tests/path");
		remote remote3("AnotherRemote", remote::sftp, "/another/path");

		BOOST_CHECK(remote1 == remote2);
		BOOST_CHECK(remote1 != remote3);
	}

	BOOST_AUTO_TEST_CASE(testRemoteOutputOperator)
	{
		remote remote("TestRemote", remote::google_drive, "/tests/path");

		ostringstream oss;
		oss << remote;

		BOOST_CHECK(!oss.str().empty());
	}

	BOOST_AUTO_TEST_CASE(testRemoteSetters)
	{
		remote remote("TestRemote", remote::google_drive, "/tests/path");

		remote.set_name("NewName");
		remote.set_type(remote::sftp);
		remote.set_path("/new/path");

		BOOST_CHECK_EQUAL(remote.name(), "NewName");
		BOOST_CHECK_EQUAL(remote.type(), remote::sftp);
		BOOST_CHECK_EQUAL(remote.path(), "/new/path");
	}

	BOOST_AUTO_TEST_CASE(testRemoteCopyConstructor)
	{
		remote r("TestRemote", remote::google_drive, "/tests/path");
		remote remoteCopy(r);

		BOOST_CHECK(r == remoteCopy);
	}

	BOOST_AUTO_TEST_CASE(testRemoteMoveConstructor)
	{
		remote r("TestRemote", remote::google_drive, "/tests/path");
		remote remoteCopy(std::move(r));

		BOOST_CHECK_EQUAL(r.name(), "");
		BOOST_CHECK_EQUAL(r.type(), remote::none);
		BOOST_CHECK_EQUAL(r.path(), "");
		BOOST_CHECK_EQUAL(remoteCopy.name(), "TestRemote");
		BOOST_CHECK_EQUAL(remoteCopy.type(), remote::google_drive);
		BOOST_CHECK_EQUAL(remoteCopy.path(), "/tests/path");
	}

	BOOST_AUTO_TEST_CASE(testRemoteCopyAssignment)
	{
		remote r("TestRemote", remote::google_drive, "/tests/path");
		remote remoteCopy = r;

		BOOST_CHECK(r == remoteCopy);
	}

	BOOST_AUTO_TEST_CASE(testRemoteMoveAssignment)
	{
		remote r("TestRemote", remote::google_drive, "/tests/path");
		remote remoteCopy = std::move(r);

		BOOST_CHECK_EQUAL(remoteCopy.name(), "TestRemote");
		BOOST_CHECK_EQUAL(remoteCopy.type(), remote::google_drive);
		BOOST_CHECK_EQUAL(remoteCopy.path(), "/tests/path");
	}

	BOOST_AUTO_TEST_CASE(testRemoteMoveAssignmentSetter)
	{
		remote r("TestRemote", remote::google_drive, "/tests/path");
		remote remoteCopy = std::move(r);

		BOOST_CHECK_EQUAL(remoteCopy.name(), "TestRemote");
		BOOST_CHECK_EQUAL(remoteCopy.type(), remote::google_drive);
		BOOST_CHECK_EQUAL(remoteCopy.path(), "/tests/path");

		std::string name = "NewName";

		remoteCopy.set_name(name);

		BOOST_CHECK_EQUAL(remoteCopy.name(), "NewName");

		BOOST_CHECK_EQUAL(name, remoteCopy.name());
	}

	BOOST_AUTO_TEST_CASE(testRemoteRootAndFullPath)
	{
		remote r("TestRemote", remote::google_drive, "/tests/path");

		BOOST_CHECK_EQUAL(r.root_path(), "TestRemote:");
		BOOST_CHECK_EQUAL(r.full_path(), "TestRemote:/tests/path");

	}

	BOOST_AUTO_TEST_CASE(testRemoteNoneType)
	{
		auto r = entities::remote::create_shared_ptr("TestRemote", remote::none, "/tests/path");
		BOOST_CHECK_EQUAL(r->name(), "TestRemote");
		BOOST_CHECK_EQUAL(r->type(), remote::none);
		BOOST_CHECK_EQUAL(r->path(), "/tests/path");
		BOOST_CHECK_EQUAL(r->root_path(), "");
		BOOST_CHECK_EQUAL(r->full_path(), "/tests/path");
	}

BOOST_AUTO_TEST_SUITE_END()
