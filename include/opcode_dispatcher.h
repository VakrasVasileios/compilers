#ifndef OPCODE_DISPATCHER_H
#define OPCODE_DISPATCHER_H

#include "quad.h"
#include "../util/contract/contract.h"
#include "instruction.h"

/**
 * @brief An opcode dispatcher.
 * 
 */
class IopCodeDispatcher {
public:
    /**
     * @brief Constructs a new IopCodeDispatcher object.
     * 
     */
    IopCodeDispatcher()
    : generators {
        &IopCodeDispatcher::generate_ASSIGN,
        &IopCodeDispatcher::generate_ADD,
        &IopCodeDispatcher::generate_SUB,
        &IopCodeDispatcher::generate_MUL,
        &IopCodeDispatcher::generate_DIV,
        &IopCodeDispatcher::generate_MOD,
        &IopCodeDispatcher::generate_UMINUS,
        &IopCodeDispatcher::generate_AND,
        &IopCodeDispatcher::generate_OR,
        &IopCodeDispatcher::generate_NOT,
        &IopCodeDispatcher::generate_JUMP,
        &IopCodeDispatcher::generate_IF_EQ,
        &IopCodeDispatcher::generate_IF_NOTEQ,
        &IopCodeDispatcher::generate_IF_LESSEQ,
        &IopCodeDispatcher::generate_IF_GREATEREQ,
        &IopCodeDispatcher::generate_IF_LESS,
        &IopCodeDispatcher::generate_IF_GREATER,
        &IopCodeDispatcher::generate_CALL,
        &IopCodeDispatcher::generate_PARAM,
        &IopCodeDispatcher::generate_RETURN,
        &IopCodeDispatcher::generate_GETRETVAL,
        &IopCodeDispatcher::generate_FUNCSTART,
        &IopCodeDispatcher::generate_FUNCEND,
        &IopCodeDispatcher::generate_TABLECREATE,
        &IopCodeDispatcher::generate_TABLEGETELEM,
        &IopCodeDispatcher::generate_TABLESETELEM
    } {}
    /**
     * @brief Destroys this OpCodeDispatcher object.
     * 
     */
    ~IopCodeDispatcher() = default;
    /**
     * @brief Generates a virtual machine instruction
     * from an intermediate code quad.
     * 
     * @param quad the quad from which the virtual machine
     * instuction will be generated, not null 
     * 
     * @return a virtual machine instruction from an
     * intermediate code quad, not null
     */
    Instruction*        Generate(const Quad* quad);
private:
    Instruction*        generate_ASSIGN(const Quad* quad);
    Instruction*        generate_ADD(const Quad* quad);
    Instruction*        generate_SUB(const Quad* quad);
    Instruction*        generate_MUL(const Quad* quad);
    Instruction*        generate_DIV(const Quad* quad);
    Instruction*        generate_MOD(const Quad* quad);
    Instruction*        generate_UMINUS(const Quad* quad);
    Instruction*        generate_AND(const Quad* quad);
    Instruction*        generate_OR(const Quad* quad);
    Instruction*        generate_NOT(const Quad* quad);
    Instruction*        generate_JUMP(const Quad* quad);
    Instruction*        generate_IF_EQ(const Quad* quad);
    Instruction*        generate_IF_NOTEQ(const Quad* quad);
    Instruction*        generate_IF_LESSEQ(const Quad* quad);
    Instruction*        generate_IF_GREATEREQ(const Quad* quad);
    Instruction*        generate_IF_LESS(const Quad* quad); 
    Instruction*        generate_IF_GREATER(const Quad* quad);
    Instruction*        generate_CALL(const Quad* quad);
    Instruction*        generate_PARAM(const Quad* quad);
    Instruction*        generate_RETURN(const Quad* quad);
    Instruction*        generate_GETRETVAL(const Quad* quad);
    Instruction*        generate_FUNCSTART(const Quad* quad);
    Instruction*        generate_FUNCEND(const Quad* quad);
    Instruction*        generate_TABLECREATE(const Quad* quad);
    Instruction*        generate_TABLEGETELEM(const Quad* quad);
    Instruction*        generate_TABLESETELEM(const Quad* quad);

    typedef Instruction* (IopCodeDispatcher:: *const generator_func_t) (const Quad*);
    generator_func_t    generators[26];        
};


#endif
