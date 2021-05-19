#include "../../include/expression/string_constant.h"

#include <iostream>

std::string 
StringConstant:: get_value() const {
    return value;
}

std::ostream&
StringConstant:: LogExpression(std::ostream& os) const {
    return os << "\"" << value << "\"";
}