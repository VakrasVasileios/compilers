include_directories(${compilers_SOURCE_DIR}/include)
add_subdirectory(${compilers_SOURCE_DIR}/src/expression syntax_analysis)

add_library(syntax_analysis
    syntax_analysis.cpp
    block.cpp program_stack.cpp symbol_table.cpp syntax_analysis.cpp
)

target_link_libraries(syntax_analysis expression)