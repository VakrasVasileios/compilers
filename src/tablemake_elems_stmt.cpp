#include "../include/tablemake_elems_stmt.h"

void    
TableMakeElemsStmt:: AddElement(Expression* elem) {
    PRECONDITION(elem != nullptr);
    elements.push_front(elem);
}

std::list<Expression*>
TableMakeElemsStmt::get_elements() const {
    return elements;
}

