#define BOOST_TEST_MODULE RemoteTest

#include <boost/test/included/unit_test.hpp>
#include <iridium/entities.hpp>

using namespace iridium::rclone;
using namespace std;

BOOST_AUTO_TEST_SUITE(Suite)

	BOOST_AUTO_TEST_CASE(testRemoteEquality)
	{
		entity::remote remote1("TestRemote", entity::remote::google_drive, "/tests/path");
		entity::remote remote2("TestRemote", entity::remote::google_drive, "/tests/path");
		entity::remote remote3("AnotherRemote", entity::remote::sftp, "/another/path");

		BOOST_CHECK(remote1 == remote2);
		BOOST_CHECK(remote1 != remote3);
	}

	BOOST_AUTO_TEST_CASE(testRemoteOutputOperator)
	{
		entity::remote remote("TestRemote", entity::remote::google_drive, "/tests/path");

		ostringstream oss;
		oss << remote;

		BOOST_CHECK(!oss.str().empty());
	}

	BOOST_AUTO_TEST_CASE(testRemoteSetters)
	{
		entity::remote remote("TestRemote", entity::remote::google_drive, "/tests/path");

		remote.set_name("NewName");
		remote.set_type(entity::remote::sftp);
		remote.set_path("/new/path");

		BOOST_CHECK_EQUAL(remote.name(), "NewName");
		BOOST_CHECK_EQUAL(remote.type(), entity::remote::sftp);
		BOOST_CHECK_EQUAL(remote.path(), "/new/path");
	}

	BOOST_AUTO_TEST_CASE(testRemoteCopyConstructor)
	{
		entity::remote r("TestRemote", entity::remote::google_drive, "/tests/path");
		entity::remote remoteCopy(r);

		BOOST_CHECK(r == remoteCopy);
	}

	BOOST_AUTO_TEST_CASE(testRemoteMoveConstructor)
	{
		entity::remote r("TestRemote", entity::remote::google_drive, "/tests/path");
		entity::remote remoteCopy(std::move(r));

		BOOST_CHECK_EQUAL(r.name(), "");
		BOOST_CHECK_EQUAL(r.type(), entity::remote::none);
		BOOST_CHECK_EQUAL(r.path(), "");
		BOOST_CHECK_EQUAL(remoteCopy.name(), "TestRemote");
		BOOST_CHECK_EQUAL(remoteCopy.type(), entity::remote::google_drive);
		BOOST_CHECK_EQUAL(remoteCopy.path(), "/tests/path");
	}

	BOOST_AUTO_TEST_CASE(testRemoteCopyAssignment)
	{
		entity::remote r("TestRemote", entity::remote::google_drive, "/tests/path");
		entity::remote remoteCopy = r;

		BOOST_CHECK(r == remoteCopy);
	}

	BOOST_AUTO_TEST_CASE(testRemoteMoveAssignment)
	{
		entity::remote r("TestRemote", entity::remote::google_drive, "/tests/path");
		entity::remote remoteCopy = std::move(r);

		BOOST_CHECK_EQUAL(remoteCopy.name(), "TestRemote");
		BOOST_CHECK_EQUAL(remoteCopy.type(), entity::remote::google_drive);
		BOOST_CHECK_EQUAL(remoteCopy.path(), "/tests/path");
	}

	BOOST_AUTO_TEST_CASE(testRemoteMoveAssignmentSetter)
	{
		entity::remote r("TestRemote", entity::remote::google_drive, "/tests/path");
		entity::remote remoteCopy = std::move(r);

		BOOST_CHECK_EQUAL(remoteCopy.name(), "TestRemote");
		BOOST_CHECK_EQUAL(remoteCopy.type(), entity::remote::google_drive);
		BOOST_CHECK_EQUAL(remoteCopy.path(), "/tests/path");

		std::string name = "NewName";

		remoteCopy.set_name(name);

		BOOST_CHECK_EQUAL(remoteCopy.name(), "NewName");

		BOOST_CHECK_EQUAL(name, remoteCopy.name());
	}

BOOST_AUTO_TEST_SUITE_END()
