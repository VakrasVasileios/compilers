cmake_minimum_required(VERSION 3.12) 

project(compilers) 

include_directories(test/googletest/include) 

add_subdirectory(test/googletest) 

add_executable(parser_test test/suits/parserTest.cpp)

target_link_libraries(parser_test PRIVATE gtest)

#??
set(target_sources
    src/id/Id.cpp
    src/parser_manager/ParserManager.cpp
    src/symbol_table/Block.cpp
    src/symbol_table/symbol_table_entry/function_entry/FunctionEntry.cpp
    src/symbol_table/symbol_table_entry/SymbolTableEntry.cpp
    src/symbol_table/SymbolTable.cpp
    parser.y
    scanner.l
)

target_sources(parser_test PRIVATE target_sources)

target_compile_definitions(parser_test PRIVATE TESTING=1)