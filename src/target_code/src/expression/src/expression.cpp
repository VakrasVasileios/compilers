#include "../include/expression.h"
#include <assert.h>

namespace target_code
{
    namespace expression 
    {
    ExprType
    Expression:: get_type() const {
        return type;
    }

    std::ostream& operator << (std::ostream& os, const Expression* expression) {
        return expression->LogExpression(os);
    }

    bool IsLibraryFunction(Expression* expr) {
        PRECONDITION(expr != nullptr);
        return expr->get_type() == LIB_FUNC;
    }

    bool IsUserFunction(Expression* expr) {
        PRECONDITION(expr != nullptr);
        return expr->get_type() == USER_FUNC; 
    }

    bool IsVariable(Expression* expr) {
        PRECONDITION(expr != nullptr);
        return expr->get_type() == VAR;
    }

    bool IsConstString(Expression* expr) {
        PRECONDITION(expr != nullptr);
        return expr->get_type() == CONST_STR;
    }

    bool IsConstBool(Expression* expr) {
        PRECONDITION(expr != nullptr);
        return expr->get_type() == CONST_BOOL;
    }
    
    bool IsTableMake(Expression* expr) {
        PRECONDITION(expr != nullptr);
        return expr->get_type() == TABLE_MAKE;
    }
    }
}