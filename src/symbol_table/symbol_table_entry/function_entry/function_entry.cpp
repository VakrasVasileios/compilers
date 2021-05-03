#include "../../../../include/symbol_table/symbol_table_entry/function_entry/function_entry.h"

std::list<FormalVariableEntry*>
FunctionEntry:: get_formal_arguments() const {
    return formal_arguments;
}

unsigned int
FunctionEntry:: get_argument_count() const {
    return formal_arguments.size();
}
