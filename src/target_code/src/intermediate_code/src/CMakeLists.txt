add_library(intermediate_code
    intermediate_code.cpp
    instruction_opcodes.cpp quad.cpp
    for_stmt.cpp func_def_stmt.cpp if_stmt.cpp loop_stmt.cpp while_stmt.cpp
)

target_link_libraries(intermediate_code syntax_analysis)