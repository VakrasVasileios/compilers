add_library(cpu 
    run.cpp translate.cpp translate.h execute_instruction.cpp
    execute_instruction.h
)
target_link_libraries(cpu target_code)
target_link_libraries(cpu memory)
target_link_libraries(cpu signals)
target_link_libraries(cpu execute)