include_directories(${compilers_SOURCE_DIR}/include/virtual_machine)

include_directories(${compilers_SOURCE_DIR}/src/target_code)

add_executable(
    avm
    avm_main.cpp binary_parser.cpp
)
target_link_libraries(avm target_code)