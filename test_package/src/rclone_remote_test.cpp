#define BOOST_TEST_MODULE RemoteTest

#include <boost/test/included/unit_test.hpp>
#include <iridium/entities.hpp>

using namespace iridium::rclone;
using namespace std;

BOOST_AUTO_TEST_SUITE(Suite)

    BOOST_AUTO_TEST_CASE(testRemoteEquality)
    {
        entitie::remote remote1("TestRemote", entitie::remote::google_drive, "/tests/path");
        entitie::remote remote2("TestRemote", entitie::remote::google_drive, "/tests/path");
        entitie::remote remote3("AnotherRemote", entitie::remote::sftp, "/another/path");

        BOOST_CHECK(remote1 == remote2);
        BOOST_CHECK(remote1 != remote3);
    }

    BOOST_AUTO_TEST_CASE(testRemoteOutputOperator)
    {
        entitie::remote remote("TestRemote", entitie::remote::google_drive, "/tests/path");

        ostringstream oss;
        oss << remote;

        BOOST_CHECK(!oss.str().empty());
    }

    BOOST_AUTO_TEST_CASE(testRemoteSetters)
    {
        entitie::remote remote("TestRemote", entitie::remote::google_drive, "/tests/path");

        remote.set_name("NewName");
        remote.set_type(entitie::remote::sftp);
        remote.set_path("/new/path");

        BOOST_CHECK_EQUAL(remote.name(), "NewName");
        BOOST_CHECK_EQUAL(remote.type(), entitie::remote::sftp);
        BOOST_CHECK_EQUAL(remote.path(), "/new/path");
    }

    BOOST_AUTO_TEST_CASE(testRemoteCopyConstructor)
    {
        entitie::remote r("TestRemote", entitie::remote::google_drive, "/tests/path");
        entitie::remote remoteCopy(r);

        BOOST_CHECK(r == remoteCopy);
    }

    BOOST_AUTO_TEST_CASE(testRemoteMoveConstructor)
    {
        entitie::remote r("TestRemote", entitie::remote::google_drive, "/tests/path");
        entitie::remote remoteCopy(std::move(r));

        BOOST_CHECK_EQUAL(remoteCopy.name(), "TestRemote");
        BOOST_CHECK_EQUAL(remoteCopy.type(), entitie::remote::google_drive);
        BOOST_CHECK_EQUAL(remoteCopy.path(), "/tests/path");
    }

    BOOST_AUTO_TEST_CASE(testRemoteCopyAssignment)
    {
        entitie::remote r("TestRemote", entitie::remote::google_drive, "/tests/path");
        entitie::remote remoteCopy = r;

        BOOST_CHECK(r == remoteCopy);
    }

    BOOST_AUTO_TEST_CASE(testRemoteMoveAssignment)
    {
        entitie::remote r("TestRemote", entitie::remote::google_drive, "/tests/path");
        entitie::remote remoteCopy = std::move(r);

        BOOST_CHECK_EQUAL(remoteCopy.name(), "TestRemote");
        BOOST_CHECK_EQUAL(remoteCopy.type(), entitie::remote::google_drive);
        BOOST_CHECK_EQUAL(remoteCopy.path(), "/tests/path");
    }

    BOOST_AUTO_TEST_CASE(testRemoteMoveAssignmentSetter)
    {
        entitie::remote r("TestRemote", entitie::remote::google_drive, "/tests/path");
        entitie::remote remoteCopy = std::move(r);

        BOOST_CHECK_EQUAL(remoteCopy.name(), "TestRemote");
        BOOST_CHECK_EQUAL(remoteCopy.type(), entitie::remote::google_drive);
        BOOST_CHECK_EQUAL(remoteCopy.path(), "/tests/path");


        std::string name = "NewName";

        remoteCopy.set_name(name);

        BOOST_CHECK_EQUAL(remoteCopy.name(), "NewName");

        BOOST_CHECK_EQUAL(name, remoteCopy.name());
    }

BOOST_AUTO_TEST_SUITE_END()