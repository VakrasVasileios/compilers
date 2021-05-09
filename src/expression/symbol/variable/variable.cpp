#include "../../../../include/expression/symbol/variable/variable.h"
#include <assert.h>

ScopeSpace
Variable:: get_space() const {
    return space;
}

std::string
Variable:: SpaceToString() const {
    switch (space) {
        case GLOBALVAR:
            return "global";
        case LOCALVAR:
            return "local";
        case FORMALVAR:
            return "formal";
        case HIDDEN:
            return "hidden";    
        default:
            assert(false);
    }
}

std::ostream& 
Variable:: LogSymbol(std::ostream& os) const {
    os << "[" << SpaceToString()<< " " << TypeToString() << "]"
       << " \"" << get_id() << "\""
       << " (line " << get_line() << ")" 
       << " (scope " << get_scope() << ")";

    return os;   
}
