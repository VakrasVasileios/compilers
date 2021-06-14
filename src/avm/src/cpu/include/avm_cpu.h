#ifndef AVM_CPU_H
#define AVM_CPU_H

#include "../../memcell/include/avm_memcell.h"

#define AVM_STACKSIZE       4096
#define AVM_STACKENV_SIZE   4

namespace avm 
{
    namespace cpu 
    {
    /**
     * @brief The avm cpu program counter register.
     * 
     */
    extern unsigned             pc;
    /**
     * @brief Avm cpu registers.
     * 
     */
    extern memcell::AvmMemcell*  ax, *bx, *cx;
    /**
     * @brief Avm cpu special retval register.
     * 
     */
    extern memcell::AvmMemcell* retval;
    /**
     * @brief Avm cpu pointer register to the top of the 
     * avm stack segment.
     * 
     */
    extern unsigned              top;
    /**
     * @brief Avm cpu pointer register to the top call stack
     * frame.
     * 
     */
    extern unsigned             topsp;
    }
}


#endif