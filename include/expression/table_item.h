#ifndef TABLE_ITEM_H
#define TABLE_ITEM_H

#include "symbol.h"

#include "../../util/contract/contract.h"

/**
 * @brief A table item symbol primary expression.
 * 
 */
class TableItem : public Symbol {
public:
    /**
     * @brief Constructs a new Table Item object
     * 
     * @param address the address to be set as the
     * return address of this TableItem, not null and hidden.
     * @param item the table element of type: 
     * primary expression symbol to be copy constructed, not null.
     * @param index the index to this TableItem mapped symbol, not 
     * null
     * 
     */
    TableItem(Symbol* address, Symbol* item, Expression* index) 
    : Symbol(TABLE_ITEM, item->get_id(), item->get_line(), item->get_scope(), item->get_space(), item->get_offset()),
    address(verify_address(address)), index(verify_index(index)) {};
    /**
     * @brief Destroys this TableItem object.
     * 
     */
    ~TableItem() = default;
    /**
     * @brief Returns a read access to this TableItem returned address.
     * 
     * @return a read access to this TableItem returned address, not null
     */
    Symbol*         get_address() const;
    /**
     * @brief Returns a read access to this TableItem index.
     * 
     * @return a read access to this TableItem index, not null 
     */
    Expression*     get_index() const;
    
    std::ostream&   LogExpression(std::ostream& os) const override;
private:
    Symbol*         address;
    Expression*     index;

    Symbol*         verify_address(Symbol* address);
    Expression*     verify_index(Expression* index);
};


#endif 
