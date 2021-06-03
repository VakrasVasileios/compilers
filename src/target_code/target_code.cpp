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

    std::vector<IncompleteJump*> incomplete_jumps;

    void AddIncompleteJump(unsigned int label, unsigned int i_target_address) {
        PRECONDITION(label >= 0);
        PRECONDITION(i_target_address >= 0);
        incomplete_jumps.push_back(new IncompleteJump(label, i_target_address));
    }

    void PatchIncompleteJumps() {
        for (auto incomplete_jump : incomplete_jumps) {
            if (incomplete_jump->i_target_address == intermediate_code::quads.size())
                instructions[incomplete_jump->label]->result->value = instructions.size();
            else
                instructions[incomplete_jump->label]->result->value = intermediate_code::quads[incomplete_jump->i_target_address]->taddress;   
        }
    }

}