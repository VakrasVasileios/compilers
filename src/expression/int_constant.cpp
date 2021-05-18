#include "../../include/expression/int_constant.h"

int 
IntConstant:: get_value() const {
    return value;
}

std::ostream&
IntConstant:: LogExpression(std::ostream& os) const {
    return os << value;
}