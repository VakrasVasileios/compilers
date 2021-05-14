#include "../../../../include/expression/symbol/variable/global_variable.h"

std::ostream&
GlobalVariable:: LogSymbol(std::ostream& os) const {
    os << "[global variable]"
       << " \"" << get_id() << "\""
       << " (line " << get_line() << ")" 
       << " (scope " << get_scope() << ")";

    return os;   
}