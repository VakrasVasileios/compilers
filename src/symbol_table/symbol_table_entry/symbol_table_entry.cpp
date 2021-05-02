#include "../../../include/symbol_table/symbol_table_entry/symbol_table_entry.h"
#include <assert.h>

std::string
SymbolTableEntry:: TypeToString(SymbolType type) const {
    switch (type) {
    case LIB_FUNC:
        return "library function";
    case USER_FUNC:
        return "user function";
    case LOCAL_VAR:
        return "local variable";
    case GLOBAL_VAR:
        return "global variable";
    case FORMAL_VAR:
        return "formal variable";        
    default:
        assert (false);
    }
}

SymbolType
SymbolTableEntry:: get_type() const {
    return type;
}

std::string
SymbolTableEntry:: get_id() const {
    return id;
}

unsigned int
SymbolTableEntry:: get_line() const {
    return line;
}

unsigned int
SymbolTableEntry:: get_scope() const {
    return scope;
}

bool
SymbolTableEntry:: is_active() const {
    return active;
}

void
SymbolTableEntry:: set_active(bool active) {
    this->active = active;
}

std::ostream&
operator<<(std::ostream& os, const SymbolTableEntry symbol_table_entry) {
    return os << "[" << symbol_table_entry.TypeToString(symbol_table_entry.get_type()) << "]"
              << " \"" << symbol_table_entry.get_id() << "\""
              << " (line " << symbol_table_entry.get_line() << ")" 
              << " (scope " << symbol_table_entry.get_scope() << ")";
}