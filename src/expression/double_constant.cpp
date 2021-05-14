#include "../../include/expression/double_constant.h"

std::ostream&
DoubleConstant:: LogExpression(std::ostream& os) const {
    return os << value;
}