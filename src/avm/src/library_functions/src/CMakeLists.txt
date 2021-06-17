add_library(library_functions
    library_functions.cpp 
)
target_link_libraries(library_functions memcell)
target_link_libraries(library_functions exec)