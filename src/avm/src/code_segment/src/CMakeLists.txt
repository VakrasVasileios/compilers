add_library(code_segment
    avm_code_segment.cpp    
)
target_link_libraries(code_segment range)
target_link_libraries(code_segment target_code)