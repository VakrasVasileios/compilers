#include "../include/expression.h"
#include <iostream>

namespace target_code
{
    namespace expression 
    {
    std::string 
    StringConstant:: get_value() const {
        return value;
    }

    std::string
    StringConstant:: to_string() const {
        return "\"" + value + "\"";
    }

    unsigned int
    StringConstant::to_unsigned() const {
        return std::stoul(value, nullptr, 0);
    }

    void
    StringConstant::Accept(ExpressionVisitor* expr_visitor) {
        PRECONDITION(expr_visitor != nullptr);
        expr_visitor->VisitStringConstant(this);    
    }

    std::ostream&
    StringConstant:: LogExpression(std::ostream& os) const {
        return os << to_string();
    }
    }
}