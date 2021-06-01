#include "../../../../../include/target_code/intermediate_code/syntax_analysis/expression/expression.h"
#include <assert.h>

namespace expression {
    ExprType
    Expression:: get_type() const {
        return type;
    }

    std::ostream& operator << (std::ostream& os, const Expression* expression) {
        return expression->LogExpression(os);
    }

    inline bool IsLibraryFunction(Expression* expr) {
        PRECONDITION(expr != nullptr);
        return expr->get_type() == LIB_FUNC;
    }

    inline bool IsUserFunction(Expression* expr) {
        PRECONDITION(expr != nullptr);
        return expr->get_type() == USER_FUNC; 
    }

    inline bool IsVariable(Expression* expr) {
        PRECONDITION(expr != nullptr);
        return expr->get_type() == VAR;
    }

    inline bool IsConstString(Expression* expr) {
        PRECONDITION(expr != nullptr);
        return expr->get_type() == CONST_STR;
    }

    inline bool IsConstBool(Expression* expr) {
        PRECONDITION(expr != nullptr);
        return expr->get_type() == CONST_BOOL;
    }
    
    inline bool IsTableMake(Expression* expr) {
        PRECONDITION(expr != nullptr);
        return expr->get_type() == TABLE_MAKE;
    }
}