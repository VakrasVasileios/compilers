#ifndef INSTRUCTION_FACTORY_H
#define INSTRUCTION_FACTORY_H

#include "../../../util/contract/include/contract.h"
#include "instruction.h"

namespace target_code
{
/**
 * @brief Creates a new Instruction instance based on a vm opcode.
 * 
 * @param op the vm opcode to base this instance creation
 * @param label the label of the new instruction object,
 * greater or equal to zero 
 * @param result the result of the new instruction object, not null
 * @param arg1 the arg1 of the new instruction object
 * @param arg2 the arg2 of the new instruction object
 * @param src_line  the source line, at which the quad, based on which the
 * new instruction object is generated, was emitted, greater than zero
 * 
 * @return a read/write access to a new Instruction instance based on
 * a vm opcode, not null
 */
Instruction*    create_instruction(Vmopcode op, const unsigned int label, Vmarg* result,
    const Vmarg* arg1, const Vmarg* arg2, const unsigned int src_line);
}

#endif