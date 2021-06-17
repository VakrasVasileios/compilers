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

        void initialize(unsigned int global_offset) {
            pc = 0;
            top = AVM_STACKSIZE - global_offset;
            topsp = top;
        }
    }
}