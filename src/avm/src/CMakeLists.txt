add_subdirectory(memcell)
add_subdirectory(code_segment)
add_subdirectory(constants)
add_subdirectory(cpu)
add_subdirectory(library_functions)
add_subdirectory(stack_segment)

add_executable(avm 
    avm_main.cpp binary_parser.cpp binary_parser.h
)
target_link_libraries(avm target_code)
target_link_libraries(avm stack_segment)
target_link_libraries(avm cpu)
target_link_libraries(avm code_segment)
target_link_libraries(avm library_functions)
target_link_libraries(avm constants)