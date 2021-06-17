#ifndef EXECUTE_INSTRUCTION_H
#define EXECUTE_INSTRUCTION_H

#include "../../../../target_code/include/instruction.h"

namespace avm
{
    namespace cpu
    {
    /**
     * @brief Executes a target code instruction. 
     * 
     * @param instr the instruction to be executed, not 
     * null
     */
    void    execute_instruction(target_code::Instruction* instr);
    }
}

#endif