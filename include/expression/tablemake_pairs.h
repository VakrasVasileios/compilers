#ifndef TABLEMAKEPAIRS_H
#define TABLEMAKEPAIRS_H

#include "tablemake.h"
#include "indexed.h"
#include "../../util/contract/contract.h"

/**
 * @brief A table make statement for storing pairs of expressions.
 * 
 */
class TableMakePairs final : public TableMake {
public:
    /**
     * @brief Constructs a new TableMakePairs object.
     * 
     * @param table the table of this TableMakePairs, not null 
     * @param indexed the indexed list of indexed elements of this
     * TableMakePairs, not null 
     */
    TableMakePairs(Symbol* table, Indexed* indexed): TableMake(table), indexed(verify_indexed(indexed)) {};
    /**
     * @brief Destroys this TableMakePairs object.
     * 
     */
    ~TableMakePairs() = default;
    /**
     * @brief Returns a read access to this TableMakePairs
     * indexed list of indexed elements.
     * 
     * @return  a read access to this TableMakePairs
     * indexed list of indexed elements, not null
     */
    Indexed*    get_indexed() const;
private:
    Indexed*    indexed;
    Indexed*    verify_indexed(Indexed* indexed);
};

#endif