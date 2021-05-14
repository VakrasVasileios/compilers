#include "../../../../include/expression/symbol/variable/local_variable.h"

std::ostream&
LocalVariable:: LogSymbol(std::ostream& os) const {
    os << "[local variable]"
       << " \"" << get_id() << "\""
       << " (line " << get_line() << ")" 
       << " (scope " << get_scope() << ")";

    return os;   
}