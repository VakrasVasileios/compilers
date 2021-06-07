include_directories(${compilers_SOURCE_DIR}/include/virtual_machine)

add_executable(
    avm
    avm_main.cpp binary_parser.cpp
)