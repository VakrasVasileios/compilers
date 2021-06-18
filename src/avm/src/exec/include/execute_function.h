#ifndef EXECUTE_FUNCTION_H
#define EXECUTE_FUNCTION_H

#include "../../../../../util/contract/include/contract.h"
#include "../../memcell/include/avm_memcell.h"

#define AVM_NUMACTUALS_OFFSET   +3
#define AVM_SAVEDPC_OFFSET      +2
#define AVM_SAVEDTOP_OFFSET     +1
#define AVM_SAVEDTOPSP_OFFSET   +0

namespace avm
{
    namespace exec
    {
    /**
     * @brief The total space that a function
     * call will tak up at the stack segment.
     * 
     */
    extern unsigned total_actuals;
    /**
     * @brief Executes a callfunc target code
     * instruction.
     * 
     * @param memcell the translated memcell
     * from the result vmarg to execute the call on,
     * not null
     */
    void execute_callfunc(memcell::AvmMemcell* memcell);
    /**
     * @brief Executes an enterfunc target code
     * instruction.
     * It's a checked runtime exception for the
     * pc to be equal to the taddress of the translated
     * memcell.
     * 
     * @param memcell the translated memcell
     * from the result vmarg to execute the enterfunc on,
     * not null 
     */
    void execute_enterfunc(memcell::AvmMemcell* memcell);
    /**
     * @brief Executes an exitfunc target code
     * instruction.
     */
    void execute_exitfunc();
    /**
     * @brief Executes a pusharg target code instruction.
     * 
     * @param memcell the translated memcell
     * from the result vmarg to execute the pusharg on,
     * not null 
     */
    void execute_pusharg(memcell::AvmMemcell* memcell);
    }
}


#endif