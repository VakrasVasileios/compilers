#ifndef BOOLEXPR_H
#define BOOLEXPR_H

#include "binary_expr.h"
#include "../../util/contract/contract.h"
#include <list>

namespace expression {
    /**
     * @brief A boolean logical binary expression.
     * 
     */
    class BoolExpr final : public BinaryExpr {
    public:
        /**
         * @brief Constructs a new Bool Expr object.
         * 
         * @param result                the result of this BoolExpr, not null
         * @param left_hand_operand     the left hand operand of the BoolExpr object, not null
         * @param right_hand_operand    the right hand operand of the BoolExpr object, not null
         */
        BoolExpr(Expression* result, Expression* left_hand_operand, Expression* right_hand_operand)
            : BinaryExpr(BOOL, result, left_hand_operand, right_hand_operand) {};
        /**
         * @brief Destroys this Bool Expr object.
         * 
         */
        ~BoolExpr() = default;
        /**
         * @brief Holds the labels of the jump/branch quads that will
         * be concluded to 'true' during the short circuit conclusion.
         * 
         */
        std::list<unsigned int> true_list;
        /**
         * @brief Holds the labels of the jump/branch quads that will
         * be concluded to 'false' during the short circuit conclusion.
         * 
         */
        std::list<unsigned int> false_list;
        
    };
}

#endif 
