link_directories(${compilers_SOURCE_DIR}/libs)
link_directories(${compilers_SOURCE_DIR}/src)
add_subdirectory(${compilers_SOURCE_DIR}/util/exec test)

add_executable(parser_test parser_test.cpp)
target_link_libraries(parser_test PRIVATE gtest)
target_link_libraries(parser_test PRIVATE src)
target_link_libraries(parser_test PRIVATE exec)
target_compile_definitions(parser_test PRIVATE TESTING=1)

add_executable(quad_test quad_test.cpp)
target_link_libraries(quad_test PRIVATE gtest)
target_link_libraries(quad_test PRIVATE src)
target_link_libraries(quad_test PRIVATE exec)
target_compile_definitions(quad_test PRIVATE TESTING=1)


