add_library(stack_segment
    avm_stack_segment.cpp
)
target_link_libraries(stack_segment range)
target_link_libraries(stack_segment target_code)
target_link_libraries(stack_segment memcell)
target_link_libraries(stack_segment cpu)