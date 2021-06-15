#include "../include/execute_cycle.h"
#include "execution_cycle.h"
#include "execute.h"

namespace avm
{
    namespace cpu
    {
    void execute_cycle() {
        if (execution_finished)
            return;
        else
        if (registers::pc == AVM_ENDING_PC) {
            execution_finished = 1;
            return;
        }
        else {
            assert (registers::pc < AVM_ENDING_PC);
            auto instruction = memory::code_segment[registers::pc];
            if (instruction->get_src_line())
                curr_line = instruction->get_src_line();
            unsigned old_pc = registers::pc;
            execute(instruction);
            if (registers::pc == old_pc)
                registers::pc++;
        }    
    }
    }
}