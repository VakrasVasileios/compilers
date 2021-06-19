add_library(execute execute_function.cpp execute_table.cpp
execute_arithmetic.cpp execute_branch.cpp execute_assign.cpp 
library_functions.cpp library_functions.h)
target_link_libraries(execute memory)