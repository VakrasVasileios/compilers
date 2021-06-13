#include "../../../../../include/target_code/intermediate_code/syntax_analysis/expression/arithmetic_expr.h"

namespace expression {
    void
    ArithmeticExpr::Accept(ExpressionVisitor* expr_visitor) {
        PRECONDITION(expr_visitor != nullptr);
        expr_visitor->VisitArithmeticExpression(this);
    }
}