add_library(cpu 
    cpu.cpp translate.cpp translate.h execute.cpp execute.h
)
target_link_libraries(cpu target_code)
target_link_libraries(cpu registers)
target_link_libraries(cpu memory)