#include "../../../include/symbol_table/symbol_table_entry/symbol_table_entry.h"
#include <assert.h>

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
    return os << "[" << symbol_table_entry.TypeToString() << "]"
              << " \"" << symbol_table_entry.get_id() << "\""
              << " (line " << symbol_table_entry.get_line() << ")" 
              << " (scope " << symbol_table_entry.get_scope() << ")";
}