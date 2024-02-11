cmake_minimum_required(VERSION 3.20)
project(rclone_cpp CXX)


set(CMAKE_CXX_STANDARD 23)

find_package(Boost)
if (NOT Boost_FOUND)
    message(FATAL_ERROR "Boost not found")
endif ()

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.hpp")

add_library(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC include)

target_link_libraries(${PROJECT_NAME} PUBLIC boost::boost)
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${HEADERS}")
install(TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION lib
        PUBLIC_HEADER DESTINATION include)