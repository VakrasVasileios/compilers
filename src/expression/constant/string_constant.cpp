#include "../../../include/expression/constant/string_constant.h"

std::ostream&
StringConstant:: LogExpression(std::ostream& os) const {
    return os << value;
}