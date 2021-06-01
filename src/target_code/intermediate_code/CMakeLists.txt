include_directories(${compilers_SOURCE_DIR}/include/target_code/intermediate_code)

add_subdirectory(syntax_analysis)

add_library(intermediate_code
    intermediate_code.cpp
    instruction_opcodes.cpp quad.cpp
    for_stmt.cpp func_def_stmt.cpp if_stmt.cpp loop_stmt.cpp while_stmt.cpp
)

target_link_libraries(intermediate_code syntax_analysis)