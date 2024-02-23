#define BOOST_TEST_MODULE RcloneFileTest
#include <boost/test/included/unit_test.hpp>
#include <iridium/entities.hpp>


using namespace iridium::rclone;
using namespace std;


BOOST_AUTO_TEST_SUITE(Suite)

BOOST_AUTO_TEST_CASE(testRcloneFileEquality) {
        auto r_ptr = entitie::entitie::remote::create_shared_ptr("TestRemote", entitie::entitie::remote::google_drive, "/tests/path");
        auto r_ptr2 = entitie::entitie::remote::create_shared_ptr("AnotherRemote", entitie::entitie::remote::sftp, "/another/path");

        entitie::file file1(nullptr,"path", 100, false, boost::posix_time::second_clock::local_time(), r_ptr);
        entitie::file file2(nullptr,"path", 100, false, boost::posix_time::second_clock::local_time(), r_ptr);
        entitie::file file3(nullptr,"path2", 100, false, boost::posix_time::second_clock::local_time(), r_ptr2);

        BOOST_CHECK(file1 == file2);
        BOOST_CHECK(file1 != file3);
}

BOOST_AUTO_TEST_CASE(testRcloneFileOutputOperator) {
        auto r_ptr = entitie::entitie::remote::create_shared_ptr("TestRemote", entitie::entitie::remote::google_drive, "/tests/path");

        entitie::file file(nullptr, "path", 100, false, boost::posix_time::second_clock::local_time(), r_ptr);

        ostringstream oss;
        oss << file;

        BOOST_CHECK(!oss.str().empty());
}

BOOST_AUTO_TEST_CASE(testRcloneFileSetters) {
        auto r_ptr = entitie::entitie::remote::create_shared_ptr("TestRemote", entitie::entitie::remote::google_drive, "/tests/path");

        entitie::file file(nullptr,"path", 100, false, boost::posix_time::second_clock::local_time(), r_ptr);

        file.set_name("path");
        file.set_remote(entitie::remote::create_shared_ptr("NewRemote", entitie::remote::sftp, "/new/path"));
        file.set_size(200);
        file.set_is_dir(true);
        file.set_mod_time(boost::posix_time::second_clock::local_time());

        BOOST_CHECK_EQUAL(file.name(), "path");
        BOOST_CHECK_EQUAL(file.remote()->name(), "NewRemote");
        BOOST_CHECK_EQUAL(file.size(), 200);
        BOOST_CHECK_EQUAL(file.is_dir(), true);
        BOOST_CHECK_EQUAL(file.mod_time(), boost::posix_time::second_clock::local_time());
}

BOOST_AUTO_TEST_CASE(testRcloneFileCopyConstructor) {
        auto r_ptr = entitie::remote::create_shared_ptr("TestRemote", entitie::remote::google_drive, "/tests/path");

        entitie::file f(nullptr,"path", 100, false, boost::posix_time::second_clock::local_time(), r_ptr);
        entitie::file fileCopy(f);

        BOOST_CHECK(f == fileCopy);
}

BOOST_AUTO_TEST_CASE(testRcloneFileMoveConstructor) {
        auto r_ptr = entitie::remote::create_shared_ptr("TestRemote", entitie::remote::google_drive, "/tests/path");

        entitie::file f(nullptr,"path", 100, false, boost::posix_time::second_clock::local_time(), r_ptr);
        entitie::file fileCopy(std::move(f));

        BOOST_CHECK_EQUAL(fileCopy.name(), "path");
        BOOST_CHECK_EQUAL(fileCopy.remote()->name(), "TestRemote");
        BOOST_CHECK_EQUAL(fileCopy.size(), 100);
        BOOST_CHECK_EQUAL(fileCopy.is_dir(), false);
        BOOST_CHECK_EQUAL(fileCopy.mod_time(), boost::posix_time::second_clock::local_time());
}

BOOST_AUTO_TEST_CASE(testRcloneFileCopyAssignment) {
        auto r_ptr = entitie::remote::create_shared_ptr("TestRemote", entitie::remote::google_drive, "/tests/path");

        entitie::file f(nullptr,"path", 100, false, boost::posix_time::second_clock::local_time(), r_ptr);
        entitie::file fileCopy = f;

        BOOST_CHECK(f == fileCopy);
}

BOOST_AUTO_TEST_CASE(testRcloneFileMoveAssignment) {
        auto r_ptr = entitie::remote::create_shared_ptr("TestRemote", entitie::remote::google_drive, "/tests/path");

        entitie::file f(nullptr,"path", 100, false, boost::posix_time::second_clock::local_time(), r_ptr);
        entitie::file fileCopy = std::move(f);

        BOOST_CHECK_EQUAL(fileCopy.name(), "path");
        BOOST_CHECK_EQUAL(fileCopy.remote()->name(), "TestRemote");
        BOOST_CHECK_EQUAL(fileCopy.size(), 100);
        BOOST_CHECK_EQUAL(fileCopy.is_dir(), false);
        BOOST_CHECK_EQUAL(fileCopy.mod_time(), boost::posix_time::second_clock::local_time());
}

BOOST_AUTO_TEST_CASE(testRcloneFileMoveAssignmentSetter) {
        auto r_ptr = entitie::remote::create_shared_ptr("TestRemote", entitie::remote::google_drive, "/tests/path");

        entitie::file f(nullptr,"path", 100, false, boost::posix_time::second_clock::local_time(), r_ptr);
        entitie::file fileCopy = std::move(f);

        BOOST_CHECK_EQUAL(fileCopy.name(), "path");
        BOOST_CHECK_EQUAL(fileCopy.remote()->name(), "TestRemote");
        BOOST_CHECK_EQUAL(fileCopy.size(), 100);
        BOOST_CHECK_EQUAL(fileCopy.is_dir(), false);
        BOOST_CHECK_EQUAL(fileCopy.mod_time(), boost::posix_time::second_clock::local_time());

        fileCopy.set_name("new");
        fileCopy.set_remote(entitie::remote::create_shared_ptr("NewRemote", entitie::remote::sftp, "/new/path"));
        fileCopy.set_size(200);
        fileCopy.set_is_dir(true);
        fileCopy.set_mod_time(boost::posix_time::second_clock::local_time());

        BOOST_CHECK_EQUAL(fileCopy.name(), "new");
        BOOST_CHECK_EQUAL(fileCopy.remote()->name(), "NewRemote");
        BOOST_CHECK_EQUAL(fileCopy.size(), 200);
        BOOST_CHECK_EQUAL(fileCopy.is_dir(), true);
        BOOST_CHECK_EQUAL(fileCopy.mod_time(), boost::posix_time::second_clock::local_time());
}

BOOST_AUTO_TEST_CASE(testRcloneFileAddChild) {
        auto r_ptr = entitie::remote::create_shared_ptr("TestRemote", entitie::remote::google_drive, "/tests/path");

        entitie::file file(nullptr,"parent", 100, true, boost::posix_time::second_clock::local_time(), r_ptr);
        file.add_child(entitie::file::create_shared_ptr(&file, "child", 100, false, boost::posix_time::second_clock::local_time(), r_ptr));

        BOOST_CHECK_EQUAL(file.nb_chilchren(), 1);
        BOOST_CHECK_EQUAL(file.parent(), nullptr);
        BOOST_CHECK_EQUAL(file.children().at(0)->parent(), &file);
        BOOST_CHECK_EQUAL(file.children().at(0)->name(), "child");
        BOOST_CHECK_EQUAL(file.children().at(0)->path(), "/parent/child");
}


BOOST_AUTO_TEST_SUITE_END()