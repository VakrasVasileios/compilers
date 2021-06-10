#ifndef AVM_CPU_H
#define AVM_CPU_H

#include "avm_memcell.h"
#include "avm_stack_segment.h"

#define AVM_STACKSIZE       4096
#define AVM_STACKENV_SIZE   4

namespace virtual_machine 
{
    namespace cpu 
    {
    /**
     * @brief The avm cpu program counter.
     * 
     */
    extern unsigned     pc;
    /**
     * @brief Avm cpu registers.
     * 
     */
    extern AvmMemcell   ax, bx, cx;
    /**
     * @brief Avm cpu special retval register.
     * 
     */
    extern AvmMemcell   retval;
    /**
     * @brief Avm cpu pointer to the top of the 
     * avm stack segment.
     * 
     */
    extern unsigned     top;
    /**
     * @brief Avm cpu pointer to the top call stack
     * frame.
     * 
     */
    extern unsigned     topsp;
    }
}


#endif