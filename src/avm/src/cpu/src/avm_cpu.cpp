#include "../include/avm_cpu.h"
#include "../../../target_code/include/instruction.h"

namespace avm
{
stack_segment::AvmStackSegment  avm_stack;
constants::Constants            constants;
unsigned                        pc = 0;
memcell::AvmMemcell             *ax, *bx, *cx;
memcell::AvmMemcell             *retval;
}