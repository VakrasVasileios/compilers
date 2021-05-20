#ifndef ARITHMETIC_EXPR_H
#define ARITHMETIC_EXPR_H

#include "binary_expr.h"

/**
 * @brief An arithmetic binary expression.
 * 
 */
class ArithmeticExpr final : public BinaryExpr {
public:
    /**
     * @brief Constructs a new ArithmeticExpr object
     * 
     * @param result                the result of this ArithmeticExpr operand, not null
     * @param left_hand_operand     the left hand operand of this ArithmeticExpr operand, not null 
     * @param right_hand_operand    the right hand operand of this ArithmeticExpr operand, not null 
     */
    ArithmeticExpr(Expression* result, Expression* left_hand_operand, Expression* right_hand_operand)
    : BinaryExpr(ARITHMETIC, result, left_hand_operand, right_hand_operand) {};
    /**
     * @brief Destroys this ArithmeticExpr object.
     * 
     */
    ~ArithmeticExpr() = default;    
};

#endif 
