#include "../../../../include/expression/symbol/function/function.h"

std::list<FormalVariable*>
Function:: get_formal_arguments() const {
    return formal_arguments;
}

std::ostream& 
Function:: LogSymbol(std::ostream& os) const {
    os << "[" << TypeToString() << "]"
       << " \"" << get_id() << "\""
       << " (line " << get_line() << ")" 
       << " (scope " << get_scope() << ")";

    return os;   
}
