#include "../include/run.h"
#include "../../signals/include/signals.h"
#include "../../registers/include/registers.h"
#include "../../memory/include/memory.h"
#include "execute_instruction.h"
#include <iostream>

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
                assert (registers::pc > AVM_ENDING_PC);
                auto instruction = memory::code_segment[registers::pc - 1];
                signals::curr_line = instruction->get_src_line();
                unsigned old_pc = registers::pc;
                std::cout << "executing "<< instruction;
                execute_instruction(instruction);
                if (registers::pc == old_pc) {
                    registers::pc++;
                }
                // else { std::cout << ", jmp pc: " << registers::pc; }

            }
        } // namespace
    
        void run() {
            do {
                execute_cycle();
            } while (!signals::execution_finished);
        }

    } // namespace cpu
} // namespace avm