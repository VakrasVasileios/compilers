#include "../../include/target_code/target_code.h"

namespace target_code {
    std::vector<Instruction*> instructions;
    IopCodeDispatcher opcode_dispatcher;

    void 
    Emit(Instruction* emitted) {
        instructions.push_back(emitted);
    }

    unsigned int NextInstructionLabel() {
        if (instructions.size() == 0)
            return 1;
        else     
            return instructions.back()->label + 1;
    }
}