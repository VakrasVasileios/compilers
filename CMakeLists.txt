cmake_minimum_required(VERSION 3.12) 

project(compilers) 

include_directories(test/googletest/include) 

add_subdirectory(test/googletest) 

#parserTest
add_executable(parser_test test/suites/parser_test.cpp)

target_link_libraries(parser_test PRIVATE gtest)

target_compile_definitions(parser_test PRIVATE TESTING=1)

#parserManagerTest
add_executable(parser_manager_test test/suites/parser_manager/parser_manager_test.cpp)

target_link_libraries(parser_manager_test PRIVATE gtest)

target_compile_definitions(parser_manager_test PRIVATE TESTING=1)

target_sources(parser_manager_test PRIVATE src/expression/expression.cpp)
target_sources(parser_manager_test PRIVATE src/parser_manager/parser_manager.cpp)
target_sources(parser_manager_test PRIVATE src/block/block.cpp)
target_sources(parser_manager_test PRIVATE src/program_stack/program_stack.cpp)
target_sources(parser_manager_test PRIVATE src/symbol_table/symbol_table.cpp)
target_sources(parser_manager_test PRIVATE src/symbol_table/symbol_table_entry/symbol_table_entry.cpp)
target_sources(parser_manager_test PRIVATE src/symbol_table/symbol_table_entry/function_entry/function_entry.cpp)
target_sources(parser_manager_test PRIVATE src/symbol_table/symbol_table_entry/variable_entry/variable_entry.cpp)

#BlockTest
add_executable(block_test test/suites/block/block_test.cpp)

target_link_libraries(block_test PRIVATE gtest)

target_compile_definitions(block_test PRIVATE TESTING=1)

target_sources(block_test PRIVATE src/block/block.cpp)
target_sources(block_test PRIVATE src/expression/expression.cpp)
target_sources(block_test PRIVATE src/symbol_table/symbol_table.cpp)
target_sources(block_test PRIVATE src/symbol_table/symbol_table_entry/symbol_table_entry.cpp)
target_sources(block_test PRIVATE src/symbol_table/symbol_table_entry/function_entry/function_entry.cpp)
target_sources(block_test PRIVATE src/symbol_table/symbol_table_entry/variable_entry/variable_entry.cpp)

#ProgramStackTest
add_executable(program_stack_test test/suites/program_stack/program_stack_test.cpp)

target_link_libraries(program_stack_test PRIVATE gtest)

target_compile_definitions(program_stack_test PRIVATE TESTING=1)

target_sources(program_stack_test PRIVATE src/program_stack/program_stack.cpp)
target_sources(program_stack_test PRIVATE src/expression/expression.cpp)
target_sources(program_stack_test PRIVATE src/block/block.cpp)
target_sources(program_stack_test PRIVATE src/symbol_table/symbol_table.cpp)
target_sources(program_stack_test PRIVATE src/symbol_table/symbol_table_entry/symbol_table_entry.cpp)
target_sources(program_stack_test PRIVATE src/symbol_table/symbol_table_entry/function_entry/function_entry.cpp)
target_sources(program_stack_test PRIVATE src/symbol_table/symbol_table_entry/variable_entry/variable_entry.cpp)

#SymbolTableTest
add_executable(symbol_table_test test/suites/symbol_table/symbol_table_test.cpp)

target_link_libraries(symbol_table_test PRIVATE gtest)

target_compile_definitions(symbol_table_test PRIVATE TESTING=1)

target_sources(symbol_table_test PRIVATE src/expression/expression.cpp)
target_sources(symbol_table_test PRIVATE src/block/block.cpp)
target_sources(symbol_table_test PRIVATE src/symbol_table/symbol_table.cpp)
target_sources(symbol_table_test PRIVATE src/symbol_table/symbol_table_entry/symbol_table_entry.cpp)
target_sources(symbol_table_test PRIVATE src/symbol_table/symbol_table_entry/function_entry/function_entry.cpp)
target_sources(symbol_table_test PRIVATE src/symbol_table/symbol_table_entry/variable_entry/variable_entry.cpp)

#quad_test

add_executable(quad_test test/suites/quad_test.cpp)

target_link_libraries(quad_test PRIVATE gtest)

target_compile_definitions(quad_test PRIVATE TESTING=1)