#include "../../../include/symbol_table/symbol_table_entry/SymbolTableEntry.h"
#include <assert.h>

std::string
SymbolTableEntry:: typeToString(SymbolType type) const {
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
SymbolTableEntry:: getType() const {
    return type;
}

std::string
SymbolTableEntry:: getId() const {
    return id;
}

unsigned int
SymbolTableEntry:: getLine() const {
    return line;
}

unsigned int
SymbolTableEntry:: getScope() const {
    return scope;
}

bool
SymbolTableEntry:: isActive() const {
    return active;
}

void
SymbolTableEntry:: setActive(bool active) {
    this->active = active;
}

std::ostream&
operator<<(std::ostream& os, const SymbolTableEntry symbolTableEntry) {
    return os << "\"" << symbolTableEntry.getId() << "\""
              << " [" << symbolTableEntry.typeToString(symbolTableEntry.getType()) << "]"
              << " (line: " << symbolTableEntry.getLine() << ")" 
              << " (scope " << symbolTableEntry.getScope() << ")";
}