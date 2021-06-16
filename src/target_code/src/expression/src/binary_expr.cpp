#include "../include/expression.h"

namespace target_code
{
    namespace expression 
    {
    Expression* 
    BinaryExpr::get_result() const {
        return result;
    }

    Expression* 
    BinaryExpr:: get_left_hand_operand() const {
        return left_hand_operand;
    }

    Expression* 
    BinaryExpr:: get_right_hand_operand() const {
        return right_hand_operand;
    }

    std::string
    BinaryExpr::to_string() const {
        return result->to_string();
    }

    unsigned int
    BinaryExpr::to_unsigned() const {
        return 0;
    }

    ExprType      
    BinaryExpr::verify_type(ExprType type) {
        PRECONDITION(type == ASSIGN || type == ARITHMETIC || type == BOOL);
        return type;
    }

    Expression*
    BinaryExpr:: verify_operand(Expression* operand) {
        PRECONDITION(operand != nullptr);
        return operand;
    }

    Expression*    
    BinaryExpr:: verify_result(Expression* result) {
        PRECONDITION(result != nullptr);
        return result;
    }

    std::ostream&   
    BinaryExpr:: LogExpression(std::ostream& os) const {
        return os << result;
    }
    }
}