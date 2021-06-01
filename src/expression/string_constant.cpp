#include "../../include/expression/string_constant.h"

#include <iostream>

namespace expression {
    std::string 
    StringConstant:: get_value() const {
        return value;
    }

    std::string
    StringConstant:: to_string() const {
        return "\"" + value + "\"";
    }

    std::ostream&
    StringConstant:: LogExpression(std::ostream& os) const {
        return os << to_string();
    }
}