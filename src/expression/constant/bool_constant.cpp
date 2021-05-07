#include "../../../include/expression/constant/bool_constant.h"

std::ostream&
BoolConstant:: LogExpression(std::ostream& os) const {
    return os << value;
}