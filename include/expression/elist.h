#ifndef ELIST_H
#define ELIST_H

#include "expression.h"
#include <list>

/**
 * @brief An expression list.
 * 
 */
class Elist {
public:
    /**
     * @brief Constructs a new Elist object.
     * 
     */
    Elist() = default;
    /**
     * @brief Destroys this Elist object.
     * 
     */
    ~Elist() = default;
    /**
     * @brief This EList list of expressions.
     * 
     */
    std::list<Expression*>  exprs;
};

#endif 