#include "table_item.h"

Symbol* 
TableItem:: get_address() const {
    return address;
}

Expression*
TableItem:: get_index() const {
    return index;
}

Symbol*
TableItem:: verify_address(Symbol* address) {
    PRECONDITION(address != nullptr);
    PRECONDITION(address->get_id().at(0) == '^');
    return address;
}

Expression*
TableItem:: verify_index(Expression* index) {
    PRECONDITION(index != nullptr);
    return index;
}


std::ostream&   
TableItem:: LogExpression(std::ostream& os) const {
    return os << address->get_id();
}




