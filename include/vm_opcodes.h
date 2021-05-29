#ifndef VM_OPCODES_H
#define VM_OPCODES_H

/**
 * @brief A virtual machine opcode.
 * 
 */
enum Vmopcode {
    ASSIGN_VM,
    ADD_VM,
    SUB_VM,
    DIV_VM,
    MOD_VM,
    CALLFUNC_VM,
    ENTERFUNC_VM,
    EXITFUNC_VM,
    JUMP_VM,
    JEQ_VM,
    JNE_VM,
    JGT_VM,
    JLT_VM,
    JGE_VM,
    NEWTABLE_VM,
    TABLEGETELEM_VM,
    TABLESETELEM_VM,
    PUSHARG_VM,
    NOP_VM
};

#endif 
