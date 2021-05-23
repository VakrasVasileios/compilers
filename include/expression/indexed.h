#ifndef INDEXED_H
#define INDEXED_H

#include "indexed_elem.h"
#include <list>

/**
 * @brief An indexed list of indexed elements.
 * 
 */
class Indexed final {
public:
    /**
     * @brief Constructs a new Indexed object.
     * 
     */
    Indexed() = default;
    /**
     * @brief Destroys this Indexed object.
     * 
     */
    ~Indexed() = default;
    /**
     * @brief This Indexed list of indexed elements.
     * 
     */
    std::list<IndexedElem*> pairs;
};

#endif 
