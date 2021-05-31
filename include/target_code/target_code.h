#ifndef TARGET_CODE_H
#define TARGET_CODE_H

#include <vector>
#include "virtual_machine/instruction.h"

/**
 * @brief Namespace for generating the target code.
 * 
 */
namespace target_code {
    /**
     * @brief A list of all the emitted target code
     * instructions.
     * 
     */
    std::vector<Instruction*>   instructions;
    /**
     * @brief Emits a target code instruction.
     * 
     * @param emitted the target code instruction to
     * be emitted, not null
     */
    void                        Emit(Instruction* emitted);
    /**
     * @brief Returns a read/write access to the label
     * after the most recently emitted instuction.
     * 
     * @return a read/write access to the label
     * after the most recently emitted instuction, greater or equal to zero 
     */
    unsigned int                NextInstructionLabel();
}

#endif
