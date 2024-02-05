add_executable(${PROJECT_NAME}_test_rclone_remote rclone_remote_test.cpp)
add_executable(${PROJECT_NAME}_test_rclone_file rclone_file_test.cpp)
add_executable(${PROJECT_NAME}_test test.cpp)

target_link_libraries(${PROJECT_NAME}_test_rclone_remote ${PROJECT_NAME} boost::boost)
target_link_libraries(${PROJECT_NAME}_test_rclone_file ${PROJECT_NAME} boost::boost)
target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME} boost::boost)