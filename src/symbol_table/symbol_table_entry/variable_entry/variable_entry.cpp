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

std::ostream&
operator<<(std::ostream& os, const VariableEntry variable_entry) {
    return os << "[" << variable_entry.ScopeTypeToString() << " " << variable_entry.TypeToString() << "]"
              << " \"" << variable_entry.get_id() << "\""
              << " (line " << variable_entry.get_line() << ")" 
              << " (scope " << variable_entry.get_scope() << ")";
}