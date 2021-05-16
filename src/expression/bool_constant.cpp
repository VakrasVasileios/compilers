#include "../../include/expression/bool_constant.h"

std::string
BoolConstant:: value_to_string() const {
    if (value)
        return "\'true\'";
    else
        return "\'false\'";    
}

std::ostream&
BoolConstant:: LogExpression(std::ostream& os) const {
    return os << value_to_string();
}