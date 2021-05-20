#include "mapped_table_item.h"

StringConstant*
MappedTableItem:: get_value() const {
    return value;
}

StringConstant*
MappedTableItem:: verify_value(StringConstant* value) {
    PRECONDITION(value);
    return value;
}


