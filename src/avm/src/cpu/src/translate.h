#ifndef TRANSLATE_H
#define TRANSLATE_H

#include "../../../../../util/contract/include/contract.h"
#include "../../../../target_code/include/vm_arg.h"
#include "../../memcell/include/avm_memcell.h"
#include "../../memory/include/memory.h"

namespace avm
{
    namespace cpu
    {
    
    /**
     * @brief Translates a vmarg operand into a memcell.
     * 
     * @param vmarg the vmarg operand to be translated,
     * not null
     * @param _register the register to write the result,
     * in case of a constant value, can be null
     * 
     * @return a read/write access to the translated memcell,
     * not null 
     */
    memcell::AvmMemcell* translate_operand(target_code::Vmarg* vmarg,
         memcell::AvmMemcell* _register);
    }
}

#endif