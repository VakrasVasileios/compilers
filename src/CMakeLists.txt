add_subdirectory(expression)

include_directories(${compilers_SOURCE_DIR}/include)

find_package(BISON 2.4.1)
find_package(FLEX 2.5.35)

BISON_TARGET(parser_trgt parser.y ${compilers_SOURCE_DIR}/build/src/parser.cpp)
FLEX_TARGET(scanner_trgt scanner.l ${compilers_SOURCE_DIR}/build/src/scanner.cpp)
ADD_FLEX_BISON_DEPENDENCY(scanner_trgt parser_trgt)

# Release mode executables
add_executable(phase3
  block.cpp instruction_opcodes.cpp program_stack.cpp quad.cpp symbol_table.cpp
  for_stmt.cpp func_def_stmt.cpp if_stmt.cpp 
  loop_stmt.cpp loop_stmt.cpp while_stmt.cpp
  ${BISON_parser_trgt_OUTPUTS}
  ${FLEX_scanner_trgt_OUTPUTS}
)
target_compile_definitions(phase3 PRIVATE LOGSYMTABLE=1)
target_compile_definitions(phase3 PRIVATE LOGQUADSTXT=1)
target_link_libraries(phase3 expression)

add_executable(r_intermediate_code
  block.cpp instruction_opcodes.cpp program_stack.cpp quad.cpp symbol_table.cpp
  for_stmt.cpp func_def_stmt.cpp if_stmt.cpp 
  loop_stmt.cpp loop_stmt.cpp while_stmt.cpp
  ${BISON_parser_trgt_OUTPUTS}
  ${FLEX_scanner_trgt_OUTPUTS}
)
target_compile_definitions(r_intermediate_code PRIVATE LOGQUADS=1)
target_link_libraries(r_intermediate_code expression)

add_executable(r_syntax_analysis
  block.cpp instruction_opcodes.cpp program_stack.cpp quad.cpp symbol_table.cpp
  for_stmt.cpp func_def_stmt.cpp if_stmt.cpp 
  loop_stmt.cpp loop_stmt.cpp while_stmt.cpp
  ${BISON_parser_trgt_OUTPUTS}
  ${FLEX_scanner_trgt_OUTPUTS}
)
target_compile_definitions(r_syntax_analysis PRIVATE LOGSYMTABLE=1)
target_link_libraries(r_syntax_analysis expression)

# Debug mode executables
add_executable(d_intermediate_code
  block.cpp instruction_opcodes.cpp program_stack.cpp quad.cpp symbol_table.cpp
  for_stmt.cpp func_def_stmt.cpp if_stmt.cpp 
  loop_stmt.cpp loop_stmt.cpp while_stmt.cpp
  ${BISON_parser_trgt_OUTPUTS}
  ${FLEX_scanner_trgt_OUTPUTS}
)
target_compile_definitions(d_intermediate_code PRIVATE LOGQUADS=1)
target_compile_definitions(d_intermediate_code PRIVATE TEST=1)
target_link_libraries(d_intermediate_code expression)

add_executable(d_syntax_analysis
  block.cpp instruction_opcodes.cpp program_stack.cpp quad.cpp symbol_table.cpp
  for_stmt.cpp func_def_stmt.cpp if_stmt.cpp 
  loop_stmt.cpp loop_stmt.cpp while_stmt.cpp
  ${BISON_parser_trgt_OUTPUTS}
  ${FLEX_scanner_trgt_OUTPUTS}
)
target_compile_definitions(d_syntax_analysis PRIVATE LOGSYMTABLE=1)
target_compile_definitions(d_syntax_analysis PRIVATE TEST=1)
target_link_libraries(d_syntax_analysis expression)