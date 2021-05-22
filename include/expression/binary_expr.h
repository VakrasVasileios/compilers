#ifndef BINARY_EXPR_H
#define BINARY_EXPR_H

#include "expression.h"
#include "../../util/contract/contract.h"

/**
 * @brief A binary expression.
 * 
 */
class BinaryExpr : public Expression {
public:
    /**
     * @brief Returns a read access to this BinaryExpr result.
     * 
     * @return a read access to this BinaryExpr result, not null 
     */
    Expression*     get_result() const;
    /**
     * @brief Returns a read access to this BinaryExpr left
     * hand operand expression.
     * 
     * @return a read access to this BinaryExpr left
     * hand operand expression, not null.
     */
    Expression*     get_left_hand_operand() const;
    /**
     * @brief Returns a read access to this BinaryExpr right
     * hand operand expression.
     * 
     * @return a read access to this BinaryExpr right
     * hand operand expression
     */
    Expression*     get_right_hand_operand() const;

    std::string     to_string() const override;
protected:
    /**
     * @brief Constructs a new BinaryExpr object.
     * 
     * @param type                  the expression type of the BinaryExpr object,
     *                              must be ARITHMETIC, BOOL or ASSIGN.
     * @param result                the result of the BinaryExpr object, not null
     * @param left_hand_operand     the left hand operand of the BinaryExpr object, not null
     * @param right_hand_operand    the right hand operand of the BinaryExpr object, not null
     */
    BinaryExpr(ExprType type, Expression* result, Expression* left_hand_operand, Expression* right_hand_operand)
        : Expression(type), result(verify_result(result)), left_hand_operand(left_hand_operand),
        right_hand_operand(right_hand_operand) {};
    /**
     * @brief Destroys this BinaryExpr object.
     * 
     */
    ~BinaryExpr() = default;
private:
    Expression*     result;
    Expression*     left_hand_operand;
    Expression*     right_hand_operand; 

    ExprType        verify_type(ExprType type);
    Expression*     verify_operand(Expression* operand);  
    Expression*     verify_result(Expression* result);   

    std::ostream&   LogExpression(std::ostream& os) const override;
};


#endif 
