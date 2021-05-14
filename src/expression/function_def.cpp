#include "../../include/expression/function_def.h"

Symbol*   
FunctionDef:: get_sym() const {
    return sym;
}

unsigned int
FunctionDef:: get_offset() const {
    return offset;
}

void 
FunctionDef:: IncreaseOffset() {
    offset++;
}

std::ostream&
FunctionDef:: LogExpression(std::ostream& os) const {
    return os << sym->get_id();
}