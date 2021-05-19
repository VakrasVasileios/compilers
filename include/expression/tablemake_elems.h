#ifndef TABLEMAKEELEMS_H
#define TABLEMAKEELEMS_H

#include "tablemake.h"
#include <list>
#include "../../util/contract/contract.h"

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
    TableMakeElems() = default;
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
    std::list<Expression*>    get_elements() const;
    /**
     * @brief Adds an element to this TableMakeElems list
     * of inserted elements.
     * 
     * @param elem the element to add to this TableMakeElems list
     * of inserted elements, not null
     */
    void                      AddElement(Expression* elem);
private:
    std::list<Expression*> elements;
};


#endif