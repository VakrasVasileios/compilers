#include "../include/registers.h"

namespace avm
{
    namespace registers
    {
    unsigned                        pc;
    memcell::AvmMemcell             *ax, *bx, *cx;
    memcell::AvmMemcell             *retval;
    unsigned                        top, topsp;

    void initialize() {
        pc = 0;
        top = AVM_STACKSIZE;
        topsp = top;
    }
    }
}