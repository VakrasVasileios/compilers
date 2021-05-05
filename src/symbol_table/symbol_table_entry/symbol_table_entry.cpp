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

void
SymbolTableEntry:: Log(std::ostream& output) {
    output  << "[" << TypeToString() << "]"
            << " \"" << get_id() << "\""
            << " (line " << get_line() << ")" 
            << " (scope " << get_scope() << ")";
}

void 
SymbolTableEntry:: LogExpr(std::ostream& output) {
    output << id;
}