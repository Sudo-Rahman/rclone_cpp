cmake_minimum_required(VERSION 3.20)
project(PackageTest CXX)

set(CMAKE_CXX_STANDARD 23)

find_package(rclone_cpp)
find_package(Boost)

add_executable(test src/test.cpp)
target_link_libraries(test rclone_cpp::rclone_cpp boost::boost)

add_executable(rclone_file_test src/rclone_file_test.cpp)
target_link_libraries(rclone_file_test rclone_cpp::rclone_cpp boost::boost)

add_executable(rclone_remote_test src/rclone_remote_test.cpp)
target_link_libraries(rclone_remote_test rclone_cpp::rclone_cpp boost::boost)

