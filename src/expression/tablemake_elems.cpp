#include "../../include/expression/tablemake_elems.h"

Elist*
TableMakeElems::get_elements() const {
    return elements;
}

Elist*
TableMakeElems::verify_elements(Elist* elements) {
    PRECONDITION(elements != nullptr);
    return elements;
}

