cmake_minimum_required(VERSION 3.25)
project(rclone_cpp CXX)


set(CMAKE_CXX_STANDARD 23)

find_package(Boost)
if (NOT Boost_FOUND)
    message(FATAL_ERROR "Boost not found")
endif ()

include_directories(
        include/iridium
        include/iridium/process
        include/iridium/entities
        include/iridium/parsers
        include/iridium/options
)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.hpp")

add_library(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE include)

target_link_libraries(${PROJECT_NAME} PRIVATE boost::boost)
install(
        DIRECTORY include/iridium
        DESTINATION include/
)
install(
        TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION lib
)
