include_directories(${compilers_SOURCE_DIR}/include/expression)

add_library(expression expression.cpp symbol.cpp function.cpp
    local_variable.cpp global_variable.cpp formal_variable.cpp user_function.cpp
    library_function.cpp nil_constant.cpp string_constant.cpp
    bool_constant.cpp int_constant.cpp double_constant.cpp function_call.cpp
    tablemake.cpp tablemake_elems.cpp tablemake_pairs.cpp)