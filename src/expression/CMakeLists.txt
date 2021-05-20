include_directories(${compilers_SOURCE_DIR}/include/expression)

add_library(expression
    expression.cpp
    symbol.cpp
    nil_constant.cpp string_constant.cpp bool_constant.cpp int_constant.cpp double_constant.cpp
    call.cpp
    tablemake.cpp tablemake_elems.cpp tablemake_pairs.cpp
)