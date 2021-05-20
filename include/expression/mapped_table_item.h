#ifndef MAPPED_TABLE_ITEM_H
#define MAPPED_TABLE_ITEM_H

#include "table_item.h"
#include "string_constant.h"
#include "../../util/contract/contract.h"

/**
 * @brief A mapped with a symbol table item primary expression.
 * 
 */
class MappedTableItem final : public TableItem {
public:
    /**
     * @brief Returns a read access to this MappedTableItem
     * mapped value.
     * 
     * @return a read access to this MappedTableItem
     * mapped value, not null 
     */
    StringConstant* get_value() const;
    /**
     * @brief Constructs a new MappedTableItem object.
     * 
     * @param address the address to be set as the return address
     * of this MappedTableItem, not null and hidden
     * @param item  The key item, not null
     * @param value The mapped value, not null 
     */
    MappedTableItem(Symbol* address, Symbol* item, StringConstant* value) 
    : TableItem(address, item), value(verify_value(value)){};
    /**
     * @brief Destroys this Mapped Table Item object
     * 
     */
    ~MappedTableItem() = default;
private:
    StringConstant*     value;
    
    StringConstant*      verify_value(StringConstant* value);
};

#endif 
