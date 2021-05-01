cmake_minimum_required(VERSION 3.12) 

project(compilers) 

include_directories(test/googletest/include) 

add_subdirectory(test/googletest) 

#parserTest
add_executable(parser_test test/suites/parserTest.cpp)

target_link_libraries(parser_test PRIVATE gtest)

target_compile_definitions(parser_test PRIVATE TESTING=1)

#parserManagerTest
add_executable(parser_manager_test test/suites/parser_manager/parserManagerTest.cpp)

target_link_libraries(parser_manager_test PRIVATE gtest)

target_compile_definitions(parser_manager_test PRIVATE TESTING=1)

target_sources(parser_manager_test PRIVATE src/parser_manager/ParserManager.cpp)
target_sources(parser_manager_test PRIVATE src/block/Block.cpp)
target_sources(parser_manager_test PRIVATE src/program_stack/ProgramStack.cpp)
target_sources(parser_manager_test PRIVATE src/symbol_table/SymbolTable.cpp)
target_sources(parser_manager_test PRIVATE src/symbol_table/symbol_table_entry/SymbolTableEntry.cpp)
target_sources(parser_manager_test PRIVATE src/symbol_table/symbol_table_entry/function_entry/FunctionEntry.cpp)

#BlockTest
add_executable(block_test test/suites/block/BlockTest.cpp)

target_link_libraries(block_test PRIVATE gtest)

target_compile_definitions(block_test PRIVATE TESTING=1)

target_sources(block_test PRIVATE src/block/Block.cpp)
target_sources(block_test PRIVATE src/symbol_table/SymbolTable.cpp)
target_sources(block_test PRIVATE src/symbol_table/symbol_table_entry/SymbolTableEntry.cpp)
target_sources(block_test PRIVATE src/symbol_table/symbol_table_entry/function_entry/FunctionEntry.cpp)

#ProgramStackTest
add_executable(program_stack_test test/suites/program_stack/ProgramStackTest.cpp)

target_link_libraries(program_stack_test PRIVATE gtest)

target_compile_definitions(program_stack_test PRIVATE TESTING=1)

target_sources(program_stack_test PRIVATE src/program_stack/ProgramStack.cpp)
target_sources(program_stack_test PRIVATE src/block/Block.cpp)
target_sources(program_stack_test PRIVATE src/symbol_table/SymbolTable.cpp)
target_sources(program_stack_test PRIVATE src/symbol_table/symbol_table_entry/SymbolTableEntry.cpp)
target_sources(program_stack_test PRIVATE src/symbol_table/symbol_table_entry/function_entry/FunctionEntry.cpp)

#SymbolTableTest
add_executable(symbol_table_test test/suites/symbol_table/SymbolTableTest.cpp)

target_link_libraries(symbol_table_test PRIVATE gtest)

target_compile_definitions(symbol_table_test PRIVATE TESTING=1)

target_sources(symbol_table_test PRIVATE src/block/Block.cpp)
target_sources(symbol_table_test PRIVATE src/symbol_table/SymbolTable.cpp)
target_sources(symbol_table_test PRIVATE src/symbol_table/symbol_table_entry/SymbolTableEntry.cpp)
target_sources(symbol_table_test PRIVATE src/symbol_table/symbol_table_entry/function_entry/FunctionEntry.cpp)