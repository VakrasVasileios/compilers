#include "../../include/target_code/target_code.h"
#include "../../include/target_code/program_consts.h"
#include <iostream>
#include <fstream>

// #define MAGIC_NUMBER    340200501

namespace target_code {
    std::vector<Instruction*> instructions;
    IopCodeDispatcher opcode_dispatcher;

    void 
    Emit(Instruction* emitted) {
        instructions.push_back(emitted);
    }

    void LogInstructions(std::ostream& output) {
        for (auto instruction : instructions)
            output << *instruction;
    }

    void LogProgramConsts(std::ostream& output) {
        output << ProgramConsts::GetInstance();
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
        const unsigned int magic_num = 340200501;

        auto write_unsigned = [&bin_file](unsigned int target) {
            bin_file.write((char*)&target, sizeof(unsigned int));
        };
        auto write_byte = [&bin_file](uint8_t target) {
            bin_file.write((char*)&target, sizeof(uint8_t));
        };
        auto write_double = [&bin_file](double target) {
            bin_file.write((char*)&target, sizeof(double));
        };
        auto write_string = [&bin_file](std::string& target) {
            bin_file.write(target.c_str(), target.size()+1);
        };

        write_unsigned(magic_num);

        // write string array to binary file
        auto str_arr = ProgramConsts:: GetInstance().GetStringArray();
        write_unsigned(str_arr->size());
        for (auto i : *str_arr) {
            write_string(i);
        }
        // write number array to binary file
        auto num_arr = ProgramConsts:: GetInstance().GetNumberArray();
        write_unsigned(num_arr->size());
        for (auto i : *num_arr) {
            write_double(i);
        }
        // write libfunc array to binary file
        auto lfunc_arr = ProgramConsts:: GetInstance().GetLibFuncArray();
        write_unsigned(lfunc_arr->size());
        for (auto i : *lfunc_arr) {
            write_string(i);
        }
        // write userfunc array to binary file
        auto ufunc_arr = ProgramConsts:: GetInstance().GetUserFuncArray();
        write_unsigned(ufunc_arr->size());
        for (auto i : *ufunc_arr) {
            write_unsigned(i.taddress);
            write_unsigned(i.local_count);
            write_unsigned(i.total_args);
            write_string(i.id);
        }
        // write instructions to binary file
        write_unsigned(instructions.size());
        for (auto i : instructions) {
            write_byte(i->opcode);
            write_byte(i->result->type);
            write_unsigned(i->result->value);
            if (i->arg1 != nullptr) {
                write_byte(i->arg1->type);
                write_unsigned(i->arg1->value);
            }
            if (i->arg2 != nullptr) {
                write_byte(i->arg2->type);
                write_unsigned(i->arg2->value);
            }
        }

        bin_file.close();
    }
    
    std::vector<IncompleteJump*> incomplete_jumps;

    void AddIncompleteJump(unsigned int label, unsigned int i_target_address) {
        PRECONDITION(label >= 0);
        PRECONDITION(i_target_address >= 0);
        incomplete_jumps.push_back(new IncompleteJump(label, i_target_address));
    }

    void PatchIncompleteJumps() {
        for (auto incomplete_jump : incomplete_jumps) {
            if (incomplete_jump->i_target_address - 1 == intermediate_code::quads.size()) 
                instructions[incomplete_jump->label - 1]->result->value = instructions.size() + 1;
            else 
                instructions[incomplete_jump->label - 1]->result->value = intermediate_code::quads[incomplete_jump->i_target_address - 1]->taddress;   
        }
    }
}

