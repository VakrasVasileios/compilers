#include "../../include/expression/double_constant.h"

namespace expression {
    double
    DoubleConstant:: get_value() const {
        return value;
    }

    std::string     
    DoubleConstant::to_string() const {
        return std::to_string(value);
    }

    std::ostream&
    DoubleConstant:: LogExpression(std::ostream& os) const {
        return os << value;
    }
}