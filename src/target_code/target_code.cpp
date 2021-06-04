#include "../../include/target_code/target_code.h"
#include "../../include/target_code/program_consts.h"
#include <iostream>
#include <fstream>

#define MAGIC_NUMBER    340200501

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

    void
    make_binary_file(void) {
        std::fstream bin_file;
        bin_file.open("alpha.out", std::ios::binary | std::ios::out);

        bin_file << MAGIC_NUMBER;

        // write string array to binary file
        auto str_arr = ProgramConsts:: GetInstance().GetStringArray();
        bin_file << (unsigned int)str_arr->size();
        for (auto i : *str_arr) {
            bin_file << i.c_str();
        }
        // write number array to binary file
        auto num_arr = ProgramConsts:: GetInstance().GetNumberArray();
        bin_file << (unsigned int)num_arr->size();
        for (auto i : *num_arr) {
            bin_file << i;
        }
        // write libfunc array to binary file
        auto lfunc_arr = ProgramConsts:: GetInstance().GetLibFuncArray();
        bin_file << (unsigned int)lfunc_arr->size();
        for (auto i : *lfunc_arr) {
            bin_file << i.c_str();
        }
        // write userfunc array to binary file
        auto ufunc_arr = ProgramConsts:: GetInstance().GetUserFuncArray();
        bin_file << (unsigned int)ufunc_arr->size();
        for (auto i : *ufunc_arr) {
            bin_file<< i.taddress
                    << i.local_count
                    << i.total_args
                    << i.id;
        }
        // write instructions to binary file
        bin_file << (unsigned int)instructions.size();
        for (auto i : instructions) {
            bin_file<< (uint8_t)i->opcode
                    << (uint8_t)i->result->type
                    << (unsigned int)i->result->value;
            if (i->arg1 != nullptr) {
                bin_file<< (uint8_t)i->arg1->type
                        << (unsigned int)i->arg1->value;
            }
            if (i->arg2 != nullptr) {
                bin_file<< (uint8_t)i->arg2->type
                        << (unsigned int)i->arg2->value;
            }
        }

        bin_file.close();
    }
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

    std::stack<expression::Expression*> funcs;
    std::map<expression::Expression*, std::list<unsigned int>> return_labels_by_funcs;

    void BackPatchReturnList(std::list<unsigned int> return_list, unsigned int patch_label) {
        PRECONDITION(patch_label >= 0);
        for (auto result : return_list)
            instructions[result]->result->value = patch_label;
    }

}
