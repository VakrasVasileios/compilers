include_directories(${compilers_SOURCE_DIR}/include/virtual_machine)

include_directories(${compilers_SOURCE_DIR}/src/target_code)

add_subdirectory(code_segment)
add_subdirectory(cpu)
add_subdirectory(memcell)
add_subdirectory(stack_segment)

target_link_libraries(cpu memcell)
target_link_libraries(stack_segment cpu)

add_executable(
    avm
    avm_main.cpp binary_parser.cpp
)
target_link_libraries(avm target_code)