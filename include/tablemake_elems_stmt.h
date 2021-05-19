#ifndef TABLEMAKEELEMS_H
#define TABLEMAKEELEMS_H

#include "expression/expression.h"
#include <list>
#include "../util/contract/contract.h"

/**
 * @brief A table make statement for storing expressions.
 * 
 */
class TableMakeElemsStmt final {
public:
    /**
     * @brief Constructs a new TableMakeElemsStmt object.
     * 
     */
    TableMakeElemsStmt() = default;
    /**
     * @brief Destroys this TableMakeElemsStmt object.
     * 
     */
    ~TableMakeElemsStmt() = default;
    /**
     * @brief Returns a read access to this TableMakeElemsStmt list
     * of inserted elements.
     * 
     * @return a read access to this TableMakeElemsStmt list
     * of inserted elements
     */
    std::list<Expression*>    get_elements() const;
    /**
     * @brief Adds an element to this TableMakeElemsStmt list
     * of inserted elements.
     * 
     * @param elem the element to add to this TableMakeElemsStmt list
     * of inserted elements, not null
     */
    void                      AddElement(Expression* elem);
private:
    std::list<Expression*> elements;
};


#endif