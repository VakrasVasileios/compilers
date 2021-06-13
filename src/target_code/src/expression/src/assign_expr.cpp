#include "../include/expression.h"

namespace target_code 
{
    namespace expression
    {
    void
    AssignExpr::Accept(ExpressionVisitor* expr_visitor) {
        PRECONDITION(expr_visitor != nullptr);
        expr_visitor->VisitAssignExpr(this);
    }
    } 
}