#include "../include/registers.h"
#include "../../binary_parser/include/binary_parser.h"
#include "../../memory/include/stack_segment.h"

namespace avm
{
    namespace registers
    {
        unsigned                        pc;
        memcell::AvmMemcell             *ax, *bx, *cx;
        memcell::AvmMemcell             *retval;
        unsigned                        top = AVM_STACKSIZE, topsp;

        void initialize_registers() {
            ax = new memcell::UndefMemcell();
            bx = new memcell::UndefMemcell();
            cx = new memcell::UndefMemcell();
            retval = new memcell::UndefMemcell();
            pc = 1;
            topsp = top;
        }
    }
}