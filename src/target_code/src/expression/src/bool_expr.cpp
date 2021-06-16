#include "../include/expression.h"

namespace target_code
{
    namespace expression 
    {
    void 
    BoolExpr::Accept(ExpressionVisitor* expr_visitor) {
        PRECONDITION(expr_visitor != nullptr);
        expr_visitor->VisitBoolExpr(this);    
    }
    }
}