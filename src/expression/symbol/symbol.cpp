#include "../../../include/expression/symbol/symbol.h"

std::string
Symbol:: get_id() const {
    return id;
}

unsigned int
Symbol:: get_line() const {
    return line;
}

unsigned int
Symbol:: get_scope() const {
    return scope;
}

bool
Symbol:: is_active() const {
    return active;
}

void
Symbol:: set_active(bool active) {
    this->active = active;
}

std::ostream& operator<<(std::ostream& os, const Symbol* symbol) {
    return symbol->LogSymbol(os);
}

// std::ostream&
// Symbol:: LogSymbolTableEntry(std::ostream& os) const {
//     os      << "[" << TypeToString() << "]"
//             << " \"" << get_id() << "\""
//             << " (line " << get_line() << ")" 
//             << " (scope " << get_scope() << ")";
// }

std::ostream&
Symbol:: LogExpression(std::ostream& os) const {
    return os << id;
}