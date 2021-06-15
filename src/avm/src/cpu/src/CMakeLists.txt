add_library(cpu 
    cpu.cpp operand_translator.cpp operand_translator.h
)
target_link_libraries(cpu target_code)
target_link_libraries(cpu registers)
target_link_libraries(cpu memory)