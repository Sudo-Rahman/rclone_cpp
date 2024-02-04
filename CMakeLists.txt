cmake_minimum_required(VERSION 3.26)
project(Rclone++)

set(CMAKE_CXX_STANDARD 23)

find_package(Boost)

include_directories(src)


file(GLOB_RECURSE SRC
        src/*.cpp
        src/*.h
        src/*.hpp
)


add_library(${PROJECT_NAME} ${SRC})

add_executable(${PROJECT_NAME}_test_rclone_remote test/rclone_remote_test.cpp)
add_executable(${PROJECT_NAME}_test_rclone_file test/rclone_file_test.cpp)

target_link_libraries(${PROJECT_NAME} boost::boost)

target_link_libraries(${PROJECT_NAME}_test_rclone_remote ${PROJECT_NAME} boost::boost)
target_link_libraries(${PROJECT_NAME}_test_rclone_file ${PROJECT_NAME} boost::boost)
