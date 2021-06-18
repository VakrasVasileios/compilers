#include "../include/registers.h"
#include "../../binary_parser/include/binary_parser.h"

namespace avm
{
    namespace registers
    {
        unsigned int global_offset;
        unsigned                        pc;
        memcell::AvmMemcell             *ax, *bx, *cx;
        memcell::AvmMemcell             *retval;
        unsigned                        top, topsp;

        void initialize_registers(unsigned int top_value) {
            PRECONDITION(util::range::in_range<unsigned>(top_value, 0,
                AVM_STACKSIZE));
                
            pc = 1;
            top = AVM_STACKSIZE - top_value;
            topsp = top;
        }
    }
}