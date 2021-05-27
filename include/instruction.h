#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "vm_opcodes.h"
#include "vm_arg.h"


/**
 * @brief A virtual machine instruction.
 * 
 */
typedef struct instruction {
    /**
     * @brief The virtal machine opcode of this Instruction.
     * 
     */
    Vmopcode        opcode;
    /**
     * @brief The virtal machine result of this Instruction.
     * 
     */
    Vmarg           result;
    /**
     * @brief The virtal machine arg1 of this Instruction.
     * 
     */
    Vmarg           arg1;
    /**
     * @brief The virtal machine arg2 of this Instruction.
     * 
     */
    Vmarg           arg2;
    /**
     * @brief The src line where the intermediatecode quad,
     * from which this Instruction was generated, was Emitted.
     * 
     */
    unsigned int    src_line;
} Instruction;

#endif 
