include_directories(${compilers_SOURCE_DIR}/include/expression)

add_library(expression
    expression.cpp
    primary.cpp
    binary_expr.cpp
    symbol.cpp
    nil_constant.cpp string_constant.cpp bool_constant.cpp int_constant.cpp double_constant.cpp
    call.cpp
    tablemake.cpp tablemake_elems.cpp tablemake_pairs.cpp
    table_item.cpp indexed_table_item.cpp mapped_table_item.cpp
)