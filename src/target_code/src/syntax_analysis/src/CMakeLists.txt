add_library(syntax_analysis
    syntax_analysis.cpp
    block.cpp block.h program_stack.cpp program_stack.h symbol_table.cpp
    symbol_table.h
)
target_link_libraries(syntax_analysis expression)