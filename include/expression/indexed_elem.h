#ifndef INDEXED_ELEM_H
#define INDEXED_ELEM_H

#include "expression.h"

/**
 * @brief An indexed element of a pair of expressions.
 * 
 */
class IndexedElem final {
public:
    /**
     * @brief Constructs a new IndexedElem object.
     * 
     * @param pair the indexed element pair of expressions
     */
    IndexedElem(std::pair<Expression*, Expression*>  pair) : pair(pair) {};
    /**
     * @brief Destroys this IndexedElem object.
     * 
     */
    ~IndexedElem() = default;
    /**
     * @brief The indexed element pair of expressions.
     * 
     */
    std::pair<Expression*, Expression*>  pair;
};

#endif 
