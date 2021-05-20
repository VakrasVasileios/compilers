#include "table_item.h"

Symbol* 
TableItem:: get_item() const {
    return item;
}

Symbol* 
TableItem:: get_address() const {
    return address;
}

Symbol*
TableItem:: verify_address(Symbol* address) {
    PRECONDITION(address != nullptr);
    PRECONDITION(address->get_id().at(0) == '^');
    return address;
}

Symbol* 
TableItem::verify_item(Symbol* sym) {
    PRECONDITION(sym != nullptr);
    return sym;
}

std::ostream&   
TableItem:: LogExpression(std::ostream& os) const {
    return os << address;
}

std::string
TableItem:: to_string() const {
    return address->get_id();
}


