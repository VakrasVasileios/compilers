#include "../../include/expression/bool_constant.h"

namespace expression {
    bool
    BoolConstant:: get_value() const {
        return value;
    }

    std::string     
    BoolConstant::to_string() const {
        if (value)
            return "\'true\'";
        else
            return "\'false\'";   
    }

    std::ostream&
    BoolConstant:: LogExpression(std::ostream& os) const {
        return os << to_string();
    }
}