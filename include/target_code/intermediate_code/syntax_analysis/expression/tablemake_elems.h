#ifndef TABLEMAKEELEMS_H
#define TABLEMAKEELEMS_H

#include "tablemake.h"
#include "elist.h"
#include "../../../../../util/contract/contract.h"

namespace expression {
    /**
     * @brief A table make statement for storing expressions.
     * 
     */
    class TableMakeElems final : public TableMake {
    public:
        /**
         * @brief Constructs a new TableMakeElems object.
         * 
         */
        TableMakeElems(Symbol* table, Elist* elist) : TableMake(table), elements(verify_elements(elist)) {};
        /**
         * @brief Destroys this TableMakeElems object.
         * 
         */
        ~TableMakeElems() = default;
        /**
         * @brief Returns a read access to this TableMakeElems list
         * of inserted elements.
         * 
         * @return a read access to this TableMakeElems list
         * of inserted elements
         */
        Elist*  get_elements() const;
        void    Accept(ExpressionVisitor* expr_visitor) override;
    private:
        Elist*  elements;
        Elist*  verify_elements(Elist* elements);
    };
}

#endif