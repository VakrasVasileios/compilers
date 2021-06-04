#include "../../../../../include/target_code/intermediate_code/syntax_analysis/expression/double_constant.h"

namespace expression {
    double
    DoubleConstant:: get_value() const {
        return value;
    }

    std::string     
    DoubleConstant::to_string() const {
        return std::to_string(value);
    }

    unsigned int
    DoubleConstant::to_unsigned() const {
        return static_cast<unsigned int>(value);    
    }

    void
    DoubleConstant::Accept(ExpressionVisitor* expr_visitor) {
        PRECONDITION(expr_visitor != nullptr);
        expr_visitor->VisitDoubleConstant(this);
    }

    std::ostream&
    DoubleConstant:: LogExpression(std::ostream& os) const {
        return os << value;
    }
}