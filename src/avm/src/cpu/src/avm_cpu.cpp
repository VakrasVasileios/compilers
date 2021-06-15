#include "../include/avm_cpu.h"
#include "../../../target_code/include/instruction.h"

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

    namespace execute {
        void
        ExVisitor:: VisitAdd(target_code::Add* inst) {

        }

        void
        ExVisitor:: VisitAssign(target_code::Assign* inst) {
            
        }
    }
}