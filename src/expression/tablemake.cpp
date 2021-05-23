#include "../../include/expression/tablemake.h"

Symbol*
TableMake:: get_table() const {
    return table;
}

std::string
TableMake::to_string() const {
    return table->to_string();
}

std::ostream&
TableMake::LogExpression(std::ostream& os) const {
    return os << table->get_id();
}

Symbol*
TableMake:: verify_table(Symbol* table) {
    PRECONDITION(table != nullptr);
    return table;
}

