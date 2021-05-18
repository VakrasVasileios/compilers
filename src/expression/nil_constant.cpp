#include "../../include/expression/nil_constant.h"

std::nullptr_t
NilConstant:: get_value() const {
    return value;
}

std::ostream&
NilConstant:: LogExpression(std::ostream& os) const {
    return os << "NIL";
}