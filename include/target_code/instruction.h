#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "vm_opcodes.h"
#include "vm_arg.h"
#include <ostream>

namespace target_code {
    /**
     * @brief A virtual machine instruction.
     * 
     */
    typedef struct instruction {
        /**
         * @brief The label of this Instruction.
         * 
         */
        unsigned int    label;
        /**
         * @brief The virtual machine opcode of this Instruction.
         * 
         */
        Vmopcode        opcode;
        /**
         * @brief The virtual machine result of this Instruction.
         * 
         */
        Vmarg*           result;
        /**
         * @brief The virtual machine arg1 of this Instruction.
         * 
         */
        Vmarg*           arg1;
        /**
         * @brief The virtual machine arg2 of this Instruction.
         * 
         */
        Vmarg*           arg2;
        /**
         * @brief The src line where the intermediatecode quad,
         * from which this Instruction was generated, was Emitted.
         * 
         */
        unsigned int    src_line;
        /**
         * @brief Constructs a new instruction object.
         * 
         * @param label     the label of the new instruction object, 
         * @param opcode    the opcode of the new instruction object
         * @param result    the result of the new instruction object, can be null
         * @param arg1      the arg1 of the new instruction object, can be null
         * @param arg2      the arg2 of the new instruction object, can be null
         * @param src_line  the source line, at which the quad, based on which the
         * new instruction object is generated, was emitted, greater than zero
         */
        instruction(unsigned int label, Vmopcode opcode, Vmarg* result, Vmarg* arg1, Vmarg* arg2, unsigned int src_line)
        : label(label), opcode(opcode), result(result), arg1(arg1), arg2(arg2), src_line(src_line) {};

        friend std::ostream &operator<<(std::ostream &os, const instruction rhs);

    } Instruction;
}

#endif 
