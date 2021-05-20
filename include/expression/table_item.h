#ifndef TABLE_ITEM_H
#define TABLE_ITEM_H

#include "symbol.h"
#include "primary.h"

#include "../../util/contract/contract.h"

/**
 * @brief A table item primary expression.
 * 
 */
class TableItem : public Primary {
public:
    /**
     * @brief Returns a read access to this TableItem item symbol.
     * 
     * @return a read access to this TableItem item symbol, not null
     */
    Symbol* get_item() const; 
    /**
     * @brief Returns a read access to this TableItem returned address.
     * 
     * @return a read access to this TableItem returned address, not null
     */
    Symbol* get_address() const;
protected:
    /**
     * @brief Constructs a new Table Item object
     * 
     * @param address the address to be set as the
     * return address of this TableItem, not null and hidden.
     * @param item the table element of type: 
     * primary expression symbol, not null.
     * 
     */
    TableItem(Symbol* address, Symbol* item) 
    : Primary(TABLE_ITEM), address(verify_address(address)), item(verify_item(item)) {};
    /**
     * @brief Destroys this TableItem object.
     * 
     */
    ~TableItem() = default;

    std::ostream&   LogExpression(std::ostream& os) const override;
    std::string     to_string() const override;
private:
    Symbol*         address;
    Symbol*         item;

    Symbol*         verify_address(Symbol* address);
    Symbol*         verify_item(Symbol* sym);  
};


#endif 
