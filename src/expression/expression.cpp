#include "../../include/expression/expression.h"
#include <assert.h>

ExprType
Expression:: get_type() const {
    return type;
}

std::string
Expression:: TypeToString(void) const {
    switch (type) {
    case LIB_FUNC:
        return "library function";
    case USER_FUNC:
        return "user function";
    case CONST:
        return "constant";
    case VAR:
        return "variable";
    default:
        assert(false);
    }
}