add_subdirectory(expression)
add_subdirectory(statement)

include_directories(${compilers_SOURCE_DIR}/include)

add_library(src block.cpp instruction_opcodes.cpp program_stack.cpp quad.cpp symbol_table.cpp)

target_link_libraries(src expression)
target_link_libraries(src statement)