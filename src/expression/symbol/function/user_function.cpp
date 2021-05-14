#include "../../../../include/expression/symbol/function/user_function.h"

std::ostream&
UserFunction:: LogSymbol(std::ostream& os) const {
    os << "[user function]"
       << " \"" << get_id() << "\""
       << " (line " << get_line() << ")" 
       << " (scope " << get_scope() << ")";

    return os;   
}