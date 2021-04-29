cmake_minimum_required(VERSION 3.12) 

project(compilers) 

include_directories(test/googletest/include) 

add_subdirectory(test/googletest) 

add_executable(parser_test test/suites/parserTest.cpp)

target_link_libraries(parser_test PRIVATE gtest)

target_compile_definitions(parser_test PRIVATE TESTING=1)