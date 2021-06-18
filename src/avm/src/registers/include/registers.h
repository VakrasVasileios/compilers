#ifndef REGISTERS_H
#define REGISTERS_H

#include "../../../../../util/contract/include/contract.h"
#include "../../../../../util/range/include/range.h"
#include "../../memcell/include/avm_memcell.h"

#define AVM_STACKSIZE       4096
#define AVM_STACKENV_SIZE   4

namespace avm 
{
    namespace registers
    {
    /**
     * @brief The avm program counter register.
     * 
     */
    extern unsigned                         pc;
    /**
     * @brief Avm registers.
     * 
     */
    extern memcell::AvmMemcell              *ax, *bx, *cx;
    /**
     * @brief Avm special retval register.
     * 
     */
    extern memcell::AvmMemcell              *retval;
    /**
     * @brief Avm special ptrs that point at the top
     * of the stack segment and the previous call frame 
     * respectively.
     * 
     */
    extern unsigned                          top, topsp;
    /**
     * @brief Initializes the avm registers.
     * 
     * @param top_value the value to initialize the 
     * top and topsp registers, in range of 0 and
     * AVM_STACK_SIZE
     * 
     */
    void                                    initialize_registers(
                                                unsigned int top_value);
    }
}


#endif