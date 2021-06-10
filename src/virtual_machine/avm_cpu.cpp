#include "avm_cpu.h"

namespace virtual_machine 
{
unsigned     pc = 0;
AvmMemcell   ax, bx, cx;
AvmMemcell   retval;
unsigned     top = 0, topsp = 0;
}