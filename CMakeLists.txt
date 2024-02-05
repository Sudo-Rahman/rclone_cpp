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

target_link_libraries(${PROJECT_NAME} boost::boost)

add_subdirectory(tests)