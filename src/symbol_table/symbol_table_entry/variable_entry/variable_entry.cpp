#include "../../../../include/symbol_table/symbol_table_entry/variable_entry/variable_entry.h"
#include <assert.h>

std::string
VariableEntry:: ScopeTypeToString() const {
    switch (type) {
        case GLOBAL_T:
            return "global";
        case LOCAL_T:
            return "local";
        case FORMAL_T:
            return "formal";
        default:
            assert(false);
    }
}

void
VariableEntry:: Log(std::ostream& output) {
    output  << "[" << ScopeTypeToString()<< " " << TypeToString() << "]"
            << " \"" << get_id() << "\""
            << " (line " << get_line() << ")" 
            << " (scope " << get_scope() << ")";
}
