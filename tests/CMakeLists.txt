
include_directories(../include)
include_directories(../src)

add_executable(test ../test_package/src/test.cpp)
target_link_libraries(test boost::boost "/Users/sr-71/Documents/rclone_cpp/cmake-build-release/librclone_cpp.a")

add_executable(rclone_file_test ../test_package/src/rclone_file_test.cpp)
target_link_libraries(rclone_file_test boost::boost)

add_executable(rclone_remote_test ../test_package/src/rclone_remote_test.cpp)
target_link_libraries(rclone_remote_test boost::boost)

