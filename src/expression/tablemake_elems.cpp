#include "../../include/expression/tablemake_elems.h"

void    
TableMakeElems:: AddElement(Expression* elem) {
    PRECONDITION(elem != nullptr);
    elements.push_front(elem);
}

std::list<Expression*>
TableMakeElems::get_elements() const {
    return elements;
}

