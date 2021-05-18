#include "../../include/expression/symbol.h"
#include <assert.h>

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

ScopeSpace
Symbol:: get_space() const {
    return space;
}

unsigned int
Symbol:: get_offset() const {
    return offset;
}

void
Symbol:: set_active(bool active) {
    this->active = active;
}

std::string
Symbol:: space_to_string() const {
    switch (space) {
        case PROGRAM_VAR:
            return "PROGRAM_VAR";
        case FUNCTION_LOCAL:
            return "FUNCTION_LOCAL";  
        case FORMAL_ARG:
            return "FORMAL_ARG";
        default:
            assert (false);        
    }
}

std::ostream& operator<<(std::ostream& os, const Symbol* symbol) {
    return symbol->LogSymbol(os);
}

std::ostream&
Symbol:: LogExpression(std::ostream& os) const {
    return os << id;
}