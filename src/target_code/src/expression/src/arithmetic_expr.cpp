#include "../include/expression.h"

namespace target_code 
{
    namespace expression 
    {
    void
    ArithmeticExpr::Accept(ExpressionVisitor* expr_visitor) {
        PRECONDITION(expr_visitor != nullptr);
        expr_visitor->VisitArithmeticExpression(this);
    }
    }
}