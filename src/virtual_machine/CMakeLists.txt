include_directories(${compilers_SOURCE_DIR}/include/virtual_machine)

add_subdirectory(code_segment)
add_subdirectory(cpu)
add_subdirectory(memcell)
add_subdirectory(stack_segment)
add_subdirectory(library_functions)
add_subdirectory(constants)

link_directories(cpu memcell)
link_directories(stack_segment cpu)
link_directories(library_functions memcell)


add_executable(
    avm
    avm_main.cpp binary_parser.cpp
)
target_link_libraries(avm target_code)
target_link_libraries(avm constants)