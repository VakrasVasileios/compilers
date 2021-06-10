#include "avm_cpu.h"

namespace virtual_machine 
{
    namespace cpu 
    {
    unsigned    pc = 0;
    AvmMemcell  ax, bx, cx;
    AvmMemcell  retval;
    unsigned    top = AVM_STACKSIZE - 1;
    unsigned    topsp = 0;
    }
}