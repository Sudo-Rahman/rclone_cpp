#define BOOST_TEST_MODULE RemoteTest
#include <boost/test/included/unit_test.hpp>
#include <rclone_remote.hpp>

using namespace Iridium;
using namespace std;

BOOST_AUTO_TEST_SUITE(Suite)

BOOST_AUTO_TEST_CASE(testRemoteEquality) {
        rclone_remote remote1("TestRemote", GoogleDrive, "/tests/path");
        rclone_remote remote2("TestRemote", GoogleDrive, "/tests/path");
        rclone_remote remote3("AnotherRemote", Sftp, "/another/path");

        BOOST_CHECK(remote1 == remote2);
        BOOST_CHECK(remote1 != remote3);
}

BOOST_AUTO_TEST_CASE(testRemoteOutputOperator) {
        rclone_remote remote("TestRemote", GoogleDrive, "/tests/path");

        ostringstream oss;
        oss << remote;

        BOOST_CHECK(!oss.str().empty());
}

BOOST_AUTO_TEST_CASE(testRemoteSetters) {
        rclone_remote remote("TestRemote", GoogleDrive, "/tests/path");

        remote.set_name("NewName");
        remote.set_type(Sftp);
        remote.set_path("/new/path");

        BOOST_CHECK_EQUAL(remote.name(), "NewName");
        BOOST_CHECK_EQUAL(remote.type(), Sftp);
        BOOST_CHECK_EQUAL(remote.path(), "/new/path");
}

BOOST_AUTO_TEST_CASE(testRemoteCopyConstructor) {
        rclone_remote r("TestRemote", GoogleDrive, "/tests/path");
        rclone_remote remoteCopy(r);

        BOOST_CHECK(r == remoteCopy);
}

BOOST_AUTO_TEST_CASE(testRemoteMoveConstructor) {
        rclone_remote r("TestRemote", GoogleDrive, "/tests/path");
        rclone_remote remoteCopy(std::move(r));

        BOOST_CHECK_EQUAL(remoteCopy.name(), "TestRemote");
        BOOST_CHECK_EQUAL(remoteCopy.type(), GoogleDrive);
        BOOST_CHECK_EQUAL(remoteCopy.path(), "/tests/path");
}

BOOST_AUTO_TEST_CASE(testRemoteCopyAssignment) {
        rclone_remote r("TestRemote", GoogleDrive, "/tests/path");
        rclone_remote remoteCopy = r;

        BOOST_CHECK(r == remoteCopy);
}

BOOST_AUTO_TEST_CASE(testRemoteMoveAssignment) {
        rclone_remote r("TestRemote", GoogleDrive, "/tests/path");
        rclone_remote remoteCopy = std::move(r);

        BOOST_CHECK_EQUAL(remoteCopy.name(), "TestRemote");
        BOOST_CHECK_EQUAL(remoteCopy.type(), GoogleDrive);
        BOOST_CHECK_EQUAL(remoteCopy.path(), "/tests/path");
}

    BOOST_AUTO_TEST_CASE(testRemoteMoveAssignmentSetter) {
        rclone_remote r("TestRemote", GoogleDrive, "/tests/path");
        rclone_remote remoteCopy = std::move(r);

        BOOST_CHECK_EQUAL(remoteCopy.name(), "TestRemote");
        BOOST_CHECK_EQUAL(remoteCopy.type(), GoogleDrive);
        BOOST_CHECK_EQUAL(remoteCopy.path(), "/tests/path");


        std::string name = "NewName";

        remoteCopy.set_name(name);

        BOOST_CHECK_EQUAL(remoteCopy.name(), "NewName");

        BOOST_CHECK_EQUAL(name, remoteCopy.name());
    }

BOOST_AUTO_TEST_SUITE_END()