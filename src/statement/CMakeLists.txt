include_directories(${compilers_SOURCE_DIR}/include/statement)

add_library(statement statement.cpp loop_stmt.cpp while_stmt.cpp for_stmt.cpp)