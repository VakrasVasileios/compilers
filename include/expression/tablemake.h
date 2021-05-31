#ifndef TABLEMAKE_H
#define TABLEMAKE_H

#include "primary.h"
#include "symbol.h"
#include "../../util/contract/contract.h"

namespace expression {
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
        std::string     to_string() const override;
    protected:
        /**
         * @brief Constructs a new TableMake object.
         * 
         */
        TableMake(Symbol* table) : Primary(TABLE_MAKE), table(verify_table(table)) {};
        /**
         * @brief Destroys this TableMakeobject.
         * 
         */
        ~TableMake() = default;
        std::ostream&   LogExpression(std::ostream& os) const override; 
    private:
        Symbol*         table;
        Symbol*         verify_table(Symbol* table);
    };
}

#endif