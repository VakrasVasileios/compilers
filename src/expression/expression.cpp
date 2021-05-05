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
    case CONST_NUM:
        return "CONST_NUM";
    case CONST_BOOL:
        return "CONST_BOOL";
    case CONST_STR:
        return "CONST_STR";
    case NIL_t:
        return "NIL";
    case ARITH_EXPR:
        return "ARITH_EXPR";
    case BOOL_EXPR:
        return "BOOL_EXPR";
    case ASSIGN_EXPR:
        return "ASSIGN_EXPR";
    case NEWTABLE_EXPR:
        return "NEWTABLE_EXPR";
    case VAR:
        return "VAR";
    case TABLE_ITEM:
        return "TABLE_ITEM";
    default:
        assert(false);
    }
}

// void
// Expression:: LogExpr(std::ostream& output) {
// }