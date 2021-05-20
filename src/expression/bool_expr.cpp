#include "../../include/expression/bool_expr.h"

std::list<unsigned int>
BoolExpr:: get_true_list_const(void) const {
    return true_list;
}

std::list<unsigned int>
BoolExpr:: get_true_list(void) {
    return true_list;
}

std::list<unsigned int>
BoolExpr:: get_false_list_const(void) const {
    return false_list;
}

std::list<unsigned int>
BoolExpr:: get_false_list(void) {
    return false_list;
}

void
BoolExpr:: true_list_insert(unsigned int q_label) {
    true_list.push_back(q_label);
}

void
BoolExpr:: false_list_insert(unsigned int q_label) {
    false_list.push_back(q_label);
}

void
BoolExpr:: swap_lists(void) {
    std::list<unsigned int> swap = true_list;
    true_list = false_list;
    false_list = swap;
}

void
BoolExpr:: merge_with_true_list(std::list<unsigned int>& t_list) {
    true_list.merge(t_list);
}

void
BoolExpr:: merge_with_false_list(std::list<unsigned int>& f_list) {
    false_list.merge(f_list);
}
