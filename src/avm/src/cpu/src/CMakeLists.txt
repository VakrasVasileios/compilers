add_library(cpu 
    run.cpp translate.cpp translate.h execute.cpp execute.h
    execute_function.cpp execute_function.h execute_table.cpp execute_table.h
)
target_link_libraries(cpu target_code)
target_link_libraries(cpu registers)
target_link_libraries(cpu memory)
target_link_libraries(cpu library_functions)
target_link_libraries(cpu signals)