link_directories(${compilers_SOURCE_DIR}/libs)
link_directories(${compilers_SOURCE_DIR}/src)
link_directories(${compilers_SOURCE_DIR}/util)

add_subdirectory(${compilers_SOURCE_DIR}/util/exec test)

add_executable(parser_test parser_test.cpp)
target_link_libraries(parser_test PRIVATE gtest)
target_link_libraries(parser_test PRIVATE src)
target_link_libraries(parser_test PRIVATE exec)
target_compile_definitions(parser_test PRIVATE TESTING=1)

add_executable(block_test block_test.cpp)
target_link_libraries(block_test PRIVATE gtest)
target_link_libraries(block_test PRIVATE src)
target_compile_definitions(block_test PRIVATE TESTING=1)

add_executable(parser_manager_test parser_manager_test.cpp)
target_link_libraries(parser_manager_test PRIVATE gtest)
target_link_libraries(parser_manager_test PRIVATE src)
target_compile_definitions(parser_manager_test PRIVATE TESTING=1)

add_executable(program_stack_test program_stack_test.cpp)
target_link_libraries(program_stack_test PRIVATE gtest)
target_link_libraries(program_stack_test PRIVATE src)
target_compile_definitions(program_stack_test PRIVATE TESTING=1)

add_executable(quad_test quad_test.cpp)
target_link_libraries(quad_test PRIVATE gtest)
target_link_libraries(quad_test PRIVATE src)
target_link_libraries(quad_test PRIVATE exec)
target_compile_definitions(quad_test PRIVATE TESTING=1)

add_executable(symbol_table_test symbol_table_test.cpp)
target_link_libraries(symbol_table_test PRIVATE gtest)
target_link_libraries(symbol_table_test PRIVATE src)
target_compile_definitions(symbol_table_test PRIVATE TESTING=1)


