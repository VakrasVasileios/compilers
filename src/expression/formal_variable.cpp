#include "../../include/expression/formal_variable.h"

std::ostream&
FormalVariable:: LogSymbol(std::ostream& os) const {
    os << "[formal variable]"
       << " \"" << get_id() << "\""
       << " (line " << get_line() << ")" 
       << " (scope " << get_scope() << ")";

    return os;   
}