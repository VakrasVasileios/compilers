#ifndef TABLEMAKE_H
#define TABLEMAKE_H

#include "quad.h"
#include "expression/expression.h"
#include <list>
#include "../util/contract/contract.h"

/**
 * @brief A table make statement.
 * 
 */
class TableMakeStmt final {
public:
    /**
     * @brief Constructs a new TableMakeStmt object.
     * 
     */
    TableMakeStmt() = default;
    /**
     * @brief Destroys this TableMakeStmt object.
     * 
     */
    ~TableMakeStmt() = default;
    /**
     * @brief Returns a read access to this TableMakeStmt list
     * of inserted elements.
     * 
     * @return a read access to this TableMakeStmt list
     * of inserted elements
     */
    std::list<Expression*>    get_elements() const;
    /**
     * @brief Adds an element to this TableMakeStmt list
     * of inserted elements.
     * 
     * @param elem the element to add to this TableMakeStmt list
     * of inserted elements, not null
     */
    void                      AddElement(Expression* elem);
private:
    std::list<Expression*> elements;
};


#endif