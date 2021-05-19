#include "../include/tablemake_pairs_stmt.h"

std::list<std::pair<Expression*, Expression*>>
TableMakePairsStmt:: get_pairs() const {
    return pairs;
}

void
TableMakePairsStmt:: AddPair(Expression* index, Expression* value) {
    PRECONDITION(index != nullptr);
    PRECONDITION(value != nullptr);
    pairs.push_front(std::pair<Expression*, Expression*>(index, value));
}
