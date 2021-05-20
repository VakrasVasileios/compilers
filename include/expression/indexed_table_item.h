#ifndef INDEXED_TABLE_ITEM_H
#define INDEXED_TABLE_ITEM_H

#include "table_item.h"
#include "../../util/contract/contract.h"

/**
 * @brief An indexed table item primary expression.
 * 
 */
class IndexedTableItem final : public TableItem {
public:
    /**
     * @brief Returns a read access to the mapped index value
     * to this IndexedTableItem item.
     * 
     * @return a read access to the mapped index value
     * to this IndexedTableItem item, not null
     */
    Expression*     get_index() const;
    /**
     * @brief Constructs a new IndexedTableItem object.
     * 
     * @param address   the address to be set as the
     *                  return address of this IndexedTableItem, 
     *                  not null and hidden
     * @param sym       the key item, not null.
     * @param index     the mapped index value, not null.
     */
    IndexedTableItem(Symbol* address, Symbol* sym, Expression* index) 
    : TableItem(address, sym), index(verify_index(index)) {};
    /**
     * @brief Destroys this IndexedTableItem object.
     * 
     */
    ~IndexedTableItem() = default;
private:
    Expression* index;    

    Expression* verify_index(Expression* index);
};


#endif 
