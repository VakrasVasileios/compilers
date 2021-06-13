#include "../../../include/virtual_machine/cpu/avm_cpu.h"

namespace virtual_machine 
{
    namespace cpu 
    {
    unsigned                pc = 0;
    memcell::AvmMemcell*    ax, *bx, *cx;
    memcell::AvmMemcell*    retval;
    unsigned                top = AVM_STACKSIZE;
    unsigned                topsp = top;
    }
}