#ifndef ASSIGN_EXPR_H
#define ASSIGN_EXPR_H

#include "binary_expr.h"

namespace expression {
    /**
     * @brief An assign binary expression.
     * 
     */
    class AssignExpr final : public BinaryExpr {
    public:
        /**
         * @brief Constructs a new AssignExpr object
         * 
         * @param result                the result of this AssignExpr operand, not null 
         * @param left_hand_operand     the left hand operand of this AssignExpr operand, not null 
         * @param right_hand_operand    the right hand operand of this AssignExpr operand, not null 
         */
        AssignExpr(Expression* result, Expression* left_hand_operand, Expression* right_hand_operand)
        : BinaryExpr(ASSIGN, result, left_hand_operand, right_hand_operand) {};
        /**
         * @brief Destroys this AssignExpr object.
         * 
         */
        ~AssignExpr() = default;
    };
}

#endif 
