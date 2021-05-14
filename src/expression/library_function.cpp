#include "../../include/expression/library_function.h"

std::ostream&
LibraryFunction:: LogSymbol(std::ostream& os) const {
    os << "[library function]"
       << " \"" << get_id() << "\""
       << " (line " << get_line() << ")" 
       << " (scope " << get_scope() << ")";

    return os;   
}