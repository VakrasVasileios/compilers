#ifndef TABLEMAKE_H
#define TABLEMAKE_H

#include "expression.h"
#include "symbol.h"
#include "../../util/contract/contract.h"

/**
 * @brief A table make expression, for making
 * tables.
 * 
 */
class TableMake : public Expression {
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
protected:
    /**
     * @brief Constructs a new TableMake object.
     * 
     */
    TableMake() : Expression(TABLE_MAKE) {};
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