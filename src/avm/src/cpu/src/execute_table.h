#ifndef EXECUTE_TABLE_H
#define EXECUTE_TABLE_H

#include "../../memcell/include/avm_memcell.h"

namespace avm
{
    namespace cpu
    {
    /**
     * @brief Executes the tablesetelem instruction for a
     * memcell.
     * 
     * @param memcell the memcell to execute the tablesetelem,
     * the table, not null
     * @param key the key of the new element, not null
     * @param value the value of the new element, not null
     */
    void tablesetelem_memcell(memcell::AvmMemcell* memcell,
        const memcell::AvmMemcell* key, const memcell::AvmMemcell* value);
    /**
     * @brief Executes the tablegetelem instruction for a memcell.
     * 
     * @param lvalue the lvalue memcell to assign the result
     * of the operation, not null
     * @param memcell the memcell to execute the tablegetelem,
     * the table, not null
     * @param key the key of the element, not null
     * 
     * @return  a read access to the mapped to the key memcell
     * at a memcell, not null
     */
    memcell::AvmMemcell* tablegetelem_memcell(memcell::AvmMemcell* lvalue, 
        memcell::AvmMemcell* memcell, const memcell::AvmMemcell* key) ;   
    }
}

#endif