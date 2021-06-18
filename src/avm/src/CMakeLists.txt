add_subdirectory(signals)
add_subdirectory(memcell)
add_subdirectory(registers)
add_subdirectory(memory)
add_subdirectory(binary_parser)
add_subdirectory(exec)
add_subdirectory(cpu)
add_subdirectory(library_functions)

add_executable(avm 
    main.cpp
)
target_link_libraries(avm binary_parser)
target_link_libraries(avm cpu)