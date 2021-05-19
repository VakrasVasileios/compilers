#include "../../include/expression/tablemake_pairs.h"

std::list<std::pair<Expression*, Expression*>>
TableMakePairs:: get_pairs() const {
    return pairs;
}

void
TableMakePairs:: AddPair(Expression* index, Expression* value) {
    PRECONDITION(index != nullptr);
    PRECONDITION(value != nullptr);
    pairs.push_back(std::pair<Expression*, Expression*>(index, value));
}
