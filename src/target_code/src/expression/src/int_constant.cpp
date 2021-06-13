#include "../include/expression.h"

namespace target_code
{
    namespace expression 
    {
    int 
    IntConstant:: get_value() const {
        return value;
    }

    std::string
    IntConstant::to_string() const {
        return std::to_string(value);
    }

    unsigned int    
    IntConstant::to_unsigned() const {
        return value;
    }

    void
    IntConstant::Accept(ExpressionVisitor* expr_visitor) {
        PRECONDITION(expr_visitor != nullptr);
        expr_visitor->VisitIntConstant(this);    
    }

    std::ostream&
    IntConstant:: LogExpression(std::ostream& os) const {
        return os << value;
    }
    }
}