#define BOOST_TEST_MODULE RemoteTest
#include <boost/test/included/unit_test.hpp>
#include <Remote.hpp>

using namespace Iridium;

BOOST_AUTO_TEST_SUITE(Suite)

BOOST_AUTO_TEST_CASE(testRemoteEquality) {
        Remote remote1("TestRemote", GoogleDrive, "/test/path");
        Remote remote2("TestRemote", GoogleDrive, "/test/path");
        Remote remote3("AnotherRemote", Sftp, "/another/path");

        BOOST_CHECK(remote1 == remote2);
        BOOST_CHECK(remote1 != remote3);
}

BOOST_AUTO_TEST_CASE(testRemoteOutputOperator) {
        Remote remote("TestRemote", GoogleDrive, "/test/path");

        std::ostringstream oss;
        oss << remote;

        BOOST_CHECK(!oss.str().empty());
}

BOOST_AUTO_TEST_CASE(testRemoteSetters) {
        Remote remote("TestRemote", GoogleDrive, "/test/path");

        remote.setName("NewName");
        remote.setType(Sftp);
        remote.setPath("/new/path");

        BOOST_CHECK_EQUAL(remote.name(), "NewName");
        BOOST_CHECK_EQUAL(remote.type(), Sftp);
        BOOST_CHECK_EQUAL(remote.path(), "/new/path");
}

BOOST_AUTO_TEST_CASE(testRemoteCopyConstructor) {
        Remote remote("TestRemote", GoogleDrive, "/test/path");
        Remote remoteCopy(remote);

        BOOST_CHECK(remote == remoteCopy);
}

BOOST_AUTO_TEST_CASE(testRemoteMoveConstructor) {
        Remote remote("TestRemote", GoogleDrive, "/test/path");
        Remote remoteCopy(std::move(remote));

        BOOST_CHECK_EQUAL(remoteCopy.name(), "TestRemote");
        BOOST_CHECK_EQUAL(remoteCopy.type(), GoogleDrive);
        BOOST_CHECK_EQUAL(remoteCopy.path(), "/test/path");
}

BOOST_AUTO_TEST_CASE(testRemoteCopyAssignment) {
        Remote remote("TestRemote", GoogleDrive, "/test/path");
        Remote remoteCopy = remote;

        BOOST_CHECK(remote == remoteCopy);
}

BOOST_AUTO_TEST_CASE(testRemoteMoveAssignment) {
        Remote remote("TestRemote", GoogleDrive, "/test/path");
        Remote remoteCopy = std::move(remote);

        BOOST_CHECK_EQUAL(remoteCopy.name(), "TestRemote");
        BOOST_CHECK_EQUAL(remoteCopy.type(), GoogleDrive);
        BOOST_CHECK_EQUAL(remoteCopy.path(), "/test/path");
}

BOOST_AUTO_TEST_SUITE_END()