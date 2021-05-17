add_subdirectory(expression)

include_directories(${compilers_SOURCE_DIR}/include)

add_library(src block.cpp instruction_opcodes.cpp parser_manager.cpp program_stack.cpp quad.cpp symbol_table.cpp)

target_link_libraries(src expression)
