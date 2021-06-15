add_library(memory 
    memory.cpp 
    constants.cpp code_segment.cpp stack_segment.cpp
)
target_link_libraries(memory target_code)
target_link_libraries(memory memcell)
target_link_libraries(memory registers)