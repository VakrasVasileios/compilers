#include "../../../../../include/target_code/intermediate_code/syntax_analysis/expression/tablemake.h"

namespace expression {
    Symbol*
    TableMake:: get_table() const {
        return table;
    }

    std::string
    TableMake::to_string() const {
        return table->to_string();
    }

    unsigned int
    TableMake::to_unsigned() const {
        return 0;    
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
}