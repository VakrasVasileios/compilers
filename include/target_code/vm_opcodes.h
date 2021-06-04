#ifndef VM_OPCODES_H
#define VM_OPCODES_H

#include <string>

namespace target_code {
    /**
     * @brief A virtual machine opcode.
     * 
     */
    enum Vmopcode {
        CALLFUNC_VM,
        PUSHARG_VM,
        ENTERFUNC_VM,
        EXITFUNC_VM,
        JUMP_VM,
        NEWTABLE_VM,

        ADD_VM,
        SUB_VM,
        MUL_VM,
        DIV_VM,
        MOD_VM,

        JEQ_VM,
        JNE_VM,
        JGT_VM,
        JLT_VM,
        JGE_VM,
        JLE_VM,

        TABLEGETELEM_VM,
        TABLESETELEM_VM,

        ASSIGN_VM,
        NOP_VM
    };

    std::string VmopcodeToString(Vmopcode _op);
}

#endif 