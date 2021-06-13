#include "../include/expression.h"

namespace target_code
{
    namespace expression 
    {
    Elist*
    TableMakeElems::get_elements() const {
        return elements;
    }

    void
    TableMakeElems::Accept(ExpressionVisitor* expr_visitor) {
        PRECONDITION(expr_visitor != nullptr);
        expr_visitor->VisitTableMakeElems(this);    
    }

    Elist*
    TableMakeElems::verify_elements(Elist* elements) {
        PRECONDITION(elements != nullptr);
        return elements;
    }
    }
}