#ifndef TABLEMAKE_H
#define TABLEMAKE_H

#include "primary.h"
#include "symbol.h"
#include "../../util/contract/contract.h"

/**
 * @brief A table make primary expression.
 * 
 */
class TableMake : public Primary {
public:
    /**
     * @brief Returns a read access to this TableMake created table.
     * 
     * @return a read access to this TableMake created table, not null
     */
    Symbol*         get_table() const;
    /**
     * @brief Set this TableMake created table.
     * 
     * @param table_ the table to set this created table, not null 
     */
    void            set_table(Symbol* table_);
    std::string     to_string() const override;
protected:
    /**
     * @brief Constructs a new TableMake object.
     * 
     */
    TableMake() : Primary(TABLE_MAKE) {};
    /**
     * @brief Destroys this TableMakeobject.
     * 
     */
    ~TableMake() = default;
    std::ostream&   LogExpression(std::ostream& os) const override; 
private:
    Symbol*         table;
};

#endif