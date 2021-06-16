#include "../include/run.h"
#include "execution_cycle.h"
#include "execute.h"

namespace avm
{
    namespace cpu
    {
        namespace
        {
        void execute_cycle() {
        assert (!execution_finished);
        if (registers::pc == AVM_ENDING_PC) {
            execution_finished = true;
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
    
    void run() {
        do {
            execute_cycle();
        } while (!execution_finished);
    }
    }
}