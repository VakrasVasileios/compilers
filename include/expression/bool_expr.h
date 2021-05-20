#ifndef BOOLEXPR_H
#define BOOLEXPR_H

#include "binary_expr.h"
#include "../../util/contract/contract.h"
#include <list>

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

    std::list<unsigned int>     get_true_list_const(void) const;
    std::list<unsigned int>     get_true_list(void);

    std::list<unsigned int>     get_false_list_const(void) const;
    std::list<unsigned int>     get_false_list(void);

    void        true_list_insert(unsigned int q_label);
    void        false_list_insert(unsigned int q_label);
    void        swap_lists(void);
    void        merge_with_true_list(std::list<unsigned int>& t_list);
    void        merge_with_false_list(std::list<unsigned int>& f_list);

private:
    std::list<unsigned int> true_list;
    std::list<unsigned int> false_list;
};

#endif 
