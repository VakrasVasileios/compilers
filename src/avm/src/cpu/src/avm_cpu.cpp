#include "../include/avm_cpu.h"

namespace avm 
{
    namespace cpu 
    {
    unsigned                 pc = 0;
    memcell::AvmMemcell     *ax, *bx, *cx;
    memcell::AvmMemcell     *retval;
    unsigned                 top = AVM_STACKSIZE;
    unsigned                 topsp = top;
    }
}