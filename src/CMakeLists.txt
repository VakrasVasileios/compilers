add_subdirectory(expression)

include_directories(${compilers_SOURCE_DIR}/include)

add_library(src block.cpp instruction_opcodes.cpp program_stack.cpp quad.cpp symbol_table.cpp for_stmt.cpp func_def_stmt.cpp if_stmt.cpp 
loop_stmt.cpp loop_stmt.cpp while_stmt.cpp)

target_link_libraries(src expression)