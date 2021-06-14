add_subdirectory(expression)
add_subdirectory(syntax_analysis)
add_subdirectory(intermediate_code)

find_package(BISON 2.4.1)
find_package(FLEX 2.5.35)

BISON_TARGET(parser_trgt parser.y ${compilers_SOURCE_DIR}/build/target_code/src/parser.cpp)
FLEX_TARGET(scanner_trgt scanner.l ${compilers_SOURCE_DIR}/build/target_code/src/scanner.cpp)
ADD_FLEX_BISON_DEPENDENCY(scanner_trgt parser_trgt)

add_library(target_code
    target_code.cpp opcode_dispatcher.cpp program_consts.cpp
    instruction.cpp vm_arg.cpp make_operand_visitor.cpp instruction_factory.cpp
    vmarg_factory.cpp incomplete_jump.h make_operand_visitor.h program_consts.h
    target_code.h
)

target_link_libraries(target_code intermediate_code)

# Release mode executables
add_executable(phase4
  ${BISON_parser_trgt_OUTPUTS}
  ${FLEX_scanner_trgt_OUTPUTS}
)
target_compile_definitions(phase4 PRIVATE NDEBUG=1)
target_link_libraries(phase4 target_code)

# Debug mode executables
add_executable(d_intermediate_code
  ${BISON_parser_trgt_OUTPUTS}
  ${FLEX_scanner_trgt_OUTPUTS}
)
target_compile_definitions(d_intermediate_code PRIVATE LOGQUADS=1)
target_link_libraries(d_intermediate_code target_code)

add_executable(d_syntax_analysis
  ${BISON_parser_trgt_OUTPUTS}
  ${FLEX_scanner_trgt_OUTPUTS}
)
target_compile_definitions(d_syntax_analysis PRIVATE LOGSYMTABLE=1)
target_link_libraries(d_syntax_analysis target_code)

add_executable(d_target_code
  ${BISON_parser_trgt_OUTPUTS}
  ${FLEX_scanner_trgt_OUTPUTS}
)
target_compile_definitions(d_target_code PRIVATE LOGINSTRS=1)
target_link_libraries(d_target_code target_code)