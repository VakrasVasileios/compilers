#include "../../include/expression/int_constant.h"

std::ostream&
IntConstant:: LogExpression(std::ostream& os) const {
    return os << value;
}