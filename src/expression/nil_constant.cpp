#include "../../include/expression/nil_constant.h"

std::ostream&
NilConstant:: LogExpression(std::ostream& os) const {
    return os << "NIL";
}