#include "../../../../include/symbol_table/symbol_table_entry/function_entry/function_entry.h"

std::list<FormalVariableEntry*>
FunctionEntry:: get_formal_arguments() const {
    return formal_arguments;
}

std::ostream&
operator<<(std::ostream& os, const FunctionEntry function_entry) {
    return os << function_entry;
}