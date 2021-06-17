#include "../include/run.h"
#include "../../signals/include/signals.h"
#include "../../registers/include/registers.h"
#include "../../memory/include/memory.h"
#include "execute.h"

namespace avm
{
    namespace cpu
    {
        namespace
        {
        void execute_cycle() {
        assert (!signals::execution_finished);
        if (registers::pc == AVM_ENDING_PC) {
            signals::execution_finished = true;
            return;
        }
        assert (registers::pc < AVM_ENDING_PC);
        auto instruction = memory::code_segment[registers::pc];
        signals::curr_line = instruction->get_src_line();
        assert (signals::curr_line != 0);
        unsigned old_pc = registers::pc;
        execute(instruction);
        if (registers::pc == old_pc)
            registers::pc++;
        }
        }
    
    void run() {
        do {
            execute_cycle();
        } while (!signals::execution_finished);
    }
    }
}