#include "../include/expression.h"

namespace target_code
{
    namespace expression 
    {
    std::nullptr_t
    NilConstant:: get_value() const {
        return value;
    }

    std::string 
    NilConstant:: to_string() const {
        return "NIL";
    }

    unsigned int
    NilConstant::to_unsigned() const {
        return 0;
    }

    void
    NilConstant::Accept(ExpressionVisitor* expr_visitor) {
        PRECONDITION(expr_visitor != nullptr);
        expr_visitor->VisitNilConstant(this);
    }

    std::ostream&
    NilConstant:: LogExpression(std::ostream& os) const {
        return os << to_string();
    }
    }
}