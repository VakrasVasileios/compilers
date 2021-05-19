#include "../../include/expression/tablemake.h"

Symbol*
TableMake:: get_table() const {
    return table;
}

void
TableMake:: set_table(Symbol* table_) {
    PRECONDITION(table_ != nullptr);
    table = table_;
}

std::ostream&
TableMake::LogExpression(std::ostream& os) const {
    return os << table->get_id();
}

