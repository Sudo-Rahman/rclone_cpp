cmake_minimum_required(VERSION 3.25)
project(PackageTest CXX)

set(CMAKE_CXX_STANDARD 23)

find_package(rclone_cpp)

add_executable(test src/test.cpp)
target_link_libraries(test rclone_cpp::rclone_cpp )

add_executable(rclone_file_test src/rclone_file_test.cpp)
target_link_libraries(rclone_file_test rclone_cpp::rclone_cpp)

add_executable(rclone_remote_test src/rclone_remote_test.cpp)
target_link_libraries(rclone_remote_test rclone_cpp::rclone_cpp)

add_executable(rclone_json_log_test src/rclone_json_log_test.cpp)
target_link_libraries(rclone_json_log_test rclone_cpp::rclone_cpp)

add_executable(rclone_process_test src/rclone_process_test.cpp)
target_link_libraries(rclone_process_test rclone_cpp::rclone_cpp)

add_executable(rclone_process_pool_test src/rclone_process_pool_test.cpp)
target_link_libraries(rclone_process_pool_test rclone_cpp::rclone_cpp)

