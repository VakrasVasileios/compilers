add_subdirectory(intermediate_code)

include_directories(${compilers_SOURCE_DIR}/include/target_code)

find_package(BISON 2.4.1)
find_package(FLEX 2.5.35)

BISON_TARGET(parser_trgt parser.y ${compilers_SOURCE_DIR}/build/src/parser.cpp)
FLEX_TARGET(scanner_trgt scanner.l ${compilers_SOURCE_DIR}/build/src/scanner.cpp)
ADD_FLEX_BISON_DEPENDENCY(scanner_trgt parser_trgt)

add_library(target_code
    target_code.cpp opcode_dispatcher.cpp program_consts.cpp
    instruction.cpp vm_arg.cpp vm_opcodes.cpp make_operand_visitor.cpp
)

target_link_libraries(target_code intermediate_code)

# Release mode executables
add_executable(dlog
  ${BISON_parser_trgt_OUTPUTS}
  ${FLEX_scanner_trgt_OUTPUTS}
)
target_compile_definitions(dlog PRIVATE DELOG=1)
target_compile_definitions(dlog PRIVATE NDEBUG=1)
target_link_libraries(dlog target_code)

add_executable(phase3
  ${BISON_parser_trgt_OUTPUTS}
  ${FLEX_scanner_trgt_OUTPUTS}
)
target_compile_definitions(phase3 PRIVATE LOGSYMTABLE=1)
target_compile_definitions(phase3 PRIVATE LOGQUADSTXT=1)
target_compile_definitions(phase3 PRIVATE NDEBUG=1)
target_link_libraries(phase3 target_code)

add_executable(r_intermediate_code
  ${BISON_parser_trgt_OUTPUTS}
  ${FLEX_scanner_trgt_OUTPUTS}
)
target_compile_definitions(r_intermediate_code PRIVATE LOGQUADS=1)
target_compile_definitions(r_intermediate_code PRIVATE NDEBUG=1)
target_link_libraries(r_intermediate_code target_code)

add_executable(r_syntax_analysis
  ${BISON_parser_trgt_OUTPUTS}
  ${FLEX_scanner_trgt_OUTPUTS}
)
target_compile_definitions(r_syntax_analysis PRIVATE LOGSYMTABLE=1)
target_compile_definitions(r_syntax_analysis PRIVATE NDEBUG=1)
target_link_libraries(r_syntax_analysis target_code)

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

add_executable(d_binary
  ${BISON_parser_trgt_OUTPUTS}
  ${FLEX_scanner_trgt_OUTPUTS}
)
target_compile_definitions(d_binary PRIVATE LOGINSTRS=1)
target_compile_definitions(d_binary PRIVATE LOGPCONSTS=1)
target_compile_definitions(d_binary PRIVATE LOGQUADSTXT=1)
target_compile_definitions(d_binary PRIVATE LOGSYMTABLE=1)
target_link_libraries(d_binary target_code)

add_executable(d_target_code
  ${BISON_parser_trgt_OUTPUTS}
  ${FLEX_scanner_trgt_OUTPUTS}
)
target_compile_definitions(d_target_code PRIVATE LOGINSTRS=1)
target_link_libraries(d_target_code target_code)