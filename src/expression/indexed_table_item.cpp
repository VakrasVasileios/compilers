#include "indexed_table_item.h"

Expression* IndexedTableItem::get_index() const {
    return index;
}

Expression* 
IndexedTableItem:: verify_index(Expression* index) {
    PRECONDITION(index != nullptr);
    return nullptr;
}
