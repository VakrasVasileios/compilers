#ifndef EXECUTE_FUNCTION_H
#define EXECUTE_FUNCTION_H

#include "../../../../../util/contract/include/contract.h"
#include "../../memcell/include/avm_memcell.h"

namespace avm
{
    namespace cpu
    {
    /**
     * @brief Saves the environment registers to
     * the stack segment.
     * 
     */
    void call_save_environment();
    /**
     * @brief Executes a call to a memcell.
     * 
     * @param memcell the memcell to execute the call
     * , not null
     */
    void call_memcell(memcell::AvmMemcell* memcell);
    }
}


#endif