#include "../include/tablemake_stmt.h"

void    
TableMakeStmt:: AddElement(Expression* elem) {
    PRECONDITION(elem != nullptr);
    elements.push_front(elem);
}

std::list<Expression*>
TableMakeStmt::get_elements() const {
    return elements;
}

