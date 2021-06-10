#ifndef AVM_CPU_H
#define AVM_CPU_H

#include "avm_memcell.h"

namespace virtual_machine 
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
 * @brief Avm cpu pointers to the top call stack
 * frame.
 * 
 */
extern unsigned     top, topsp;
}


#endif