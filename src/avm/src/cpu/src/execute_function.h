#ifndef EXECUTE_FUNCTION_H
#define EXECUTE_FUNCTION_H

#include "../../../../../util/contract/include/contract.h"
#include "../../memcell/include/avm_memcell.h"

namespace avm
{
    namespace cpu
    {
    /**
     * @brief The total space that a function
     * call will tak up at the stack segment.
     * 
     */
    extern unsigned total_actuals;
    /**
     * @brief Saves the environment registers to
     * the stack segment.
     * 
     */
    void save_environment();
    /**
     * @brief Executes a call to a memcell.
     * 
     * @param memcell the memcell to execute the call
     * , not null
     */
    void call_memcell(memcell::AvmMemcell* memcell);
    /**
     * @brief Returns a read/write access to a previously
     * stored to the stack segment environment value.
     * 
     * @param index the index of the environment value,
     * greater or equal to zero
     * 
     * @return a read/write access to the numeric value
     * of a previously stored to the stack segment environment
     * value, greater or equal to zero
     */
    unsigned get_envvalue(unsigned index);
    }
}


#endif