#include "../../include/expression/expression.h"
#include <assert.h>

ExprType
Expression:: get_type() const {
    return type;
}

std::ostream& operator << (std::ostream& os, const Expression* expression) {
    return expression->LogExpression(os);
}