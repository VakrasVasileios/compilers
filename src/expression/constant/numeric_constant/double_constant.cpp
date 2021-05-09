#include "../../../../include/expression/constant/numeric_constant/double_constant.h"

std::ostream&
DoubleConstant:: LogExpression(std::ostream& os) const {
    return os << value;
}