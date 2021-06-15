#ifndef AVM_CPU_H
#define AVM_CPU_H

#include "../../memcell/include/avm_memcell.h"
#include "../../stack_segment/include/avm_stack_segment.h"
#include "../../constants/include/constants.h"

namespace avm 
{
/**
 * @brief The avm stack memory segment.
 * 
 */
extern stack_segment::AvmStackSegment   avm_stack;
extern constants::Constants             constants;
/**
 * @brief The avm cpu program counter register.
 * 
 */
extern unsigned                         pc;
/**
 * @brief Avm cpu registers.
 * 
 */
extern memcell::AvmMemcell*             ax, *bx, *cx;
/**
 * @brief Avm cpu special retval register.
 * 
 */
extern memcell::AvmMemcell*             retval;

    class ExVisitor : public target_code::InstructionVisitor {
        void VisitAdd(target_code::Add* inst);
        void VisitAssign(target_code::Assign* inst);
    };
}


#endif