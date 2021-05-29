#include "opcode_dispatcher.h"

/* 
The dispatch takes place because the generate functions index in this
array matches each respective opcode enum value, from which each  vm
instruction is generated.                               
*/
Instruction*    
IopCodeDispatcher::Generate(const Quad* quad){
    PRECONDITION(quad != nullptr);
    return (this->*generators[quad->op])(quad);
}

Instruction*    
IopCodeDispatcher::generate_ASSIGN(const Quad* quad) {
    assert(quad->op == ASSIGN_t);
    return new Instruction(ASSIGN_VM, make_operand(quad->result), make_operand(quad->arg1), nullptr, quad->line);
}

Instruction*    
IopCodeDispatcher::generate_ADD(const Quad* quad) {
    assert(quad->op == ADD_t);
    return new Instruction(ADD_VM, make_operand(quad->result), make_operand(quad->arg1), make_operand(quad->arg2), quad->line);
}

Instruction*
IopCodeDispatcher::generate_SUB(const Quad* quad) {
    assert(quad->op == SUB_t);
    return new Instruction(SUB_VM, make_operand(quad->result), make_operand(quad->arg1), make_operand(quad->arg2), quad->line);
}

Instruction*
IopCodeDispatcher::generate_MUL(const Quad* quad) {
    assert(quad->op == MUL_t);
    return new Instruction(MUL_VM, make_operand(quad->result), make_operand(quad->arg1), make_operand(quad->arg2), quad->line);
}

Instruction*
IopCodeDispatcher::generate_DIV(const Quad* quad) {
    assert(quad->op == DIV_t);
    return new Instruction(DIV_VM, make_operand(quad->result), make_operand(quad->arg1), make_operand(quad->arg2), quad->line);
}

Instruction*
IopCodeDispatcher::generate_MOD(const Quad* quad) {
    assert(quad->op == MOD_t);
    return new Instruction(MOD_VM, make_operand(quad->result), make_operand(quad->arg1), make_operand(quad->arg2), quad->line);
}

Instruction*
IopCodeDispatcher::generate_UMINUS(const Quad* quad) {
    assert(quad->op == UMINUS_t);
    return new Instruction(MUL_VM, make_operand(quad->result), make_operand(quad->arg1), make_operand(new IntConstant(-1)), quad->line);
}

Instruction*
IopCodeDispatcher::generate_JUMP(const Quad* quad) {
    assert(quad->op == JUMP_t);
    return new Instruction(JUMP_VM, make_operand(quad->result), nullptr, nullptr, quad->line);
}

Instruction*
IopCodeDispatcher::generate_IF_EQ(const Quad* quad) {
    assert(quad->op == IF_EQ_t);
    return new Instruction(JEQ_VM, make_operand(quad->result), make_operand(quad->arg1), make_operand(quad->arg2), quad->line);
}

Instruction*
IopCodeDispatcher::generate_IF_NOTEQ(const Quad* quad) {
    assert(quad->op == IF_NOTEQ_t);
    return new Instruction(JNE_VM, make_operand(quad->result), make_operand(quad->arg1), make_operand(quad->arg2), quad->line);
}

Instruction*    
IopCodeDispatcher::generate_IF_LESSEQ(const Quad* quad) {
    assert(quad->op == IF_LESSEQ_t);
    return new Instruction(JLE_VM, make_operand(quad->result), make_operand(quad->arg1), make_operand(quad->arg2), quad->line);
}

Instruction*
IopCodeDispatcher::generate_IF_GREATEREQ(const Quad* quad) {
    assert(quad->op == IF_GREATEREQ_t);
    return new Instruction(JGE_VM, make_operand(quad->result), make_operand(quad->arg1), make_operand(quad->arg2), quad->line);
}

Instruction*
IopCodeDispatcher::generate_IF_LESS(const Quad* quad) {
    assert(quad->op == IF_LESS_t);
    return new Instruction(JLT_VM, make_operand(quad->result), make_operand(quad->arg1), make_operand(quad->arg2), quad->line);
}

Instruction*    
IopCodeDispatcher::generate_IF_GREATER(const Quad* quad) {
    assert(quad->op == IF_GREATER_t);
    return new Instruction(JGT_VM, make_operand(quad->result), make_operand(quad->arg1), make_operand(quad->arg2), quad->line);
}

Instruction*    
IopCodeDispatcher::generate_CALL(const Quad* quad) {
    assert(quad->op == CALL_t);
    return new Instruction(CALLFUNC_VM, make_operand(quad->result), nullptr, nullptr, quad->line);
}

Instruction*
IopCodeDispatcher::generate_PARAM(const Quad* quad) {
    assert(quad->op == PARAM_t);
    return new Instruction(PUSHARG_VM, make_operand(quad->result), nullptr, nullptr, quad->line);
}

Instruction*    
IopCodeDispatcher::generate_RETURN(const Quad* quad) {
    assert(quad->op == RET_t);
    return nullptr;
}

Instruction*
IopCodeDispatcher::generate_GETRETVAL(const Quad* quad) {
    assert(quad->op == GETRETVAL_t);
    return nullptr;
}

Instruction*
IopCodeDispatcher::generate_FUNCSTART(const Quad* quad) {
    assert(quad->op == FUNCSTART_t);
    return new Instruction(ENTERFUNC_VM, make_operand(quad->result), nullptr, nullptr, quad->line);
}

Instruction*    
IopCodeDispatcher::generate_FUNCEND(const Quad* quad) {
    assert(quad->op == FUNCEND_t);
    return new Instruction(EXITFUNC_VM, make_operand(quad->result), nullptr, nullptr, quad->line);
}

Instruction*    
IopCodeDispatcher::generate_TABLECREATE(const Quad* quad) {
    assert(quad->op == TABLECREATE_t);
    return new Instruction(NEWTABLE_VM, make_operand(quad->result), nullptr, nullptr, quad->line);
}

Instruction*    
IopCodeDispatcher::generate_TABLEGETELEM(const Quad* quad) {
    assert(quad->op == TABLEGETELEM_t);
    return new Instruction(TABLEGETELEM_VM, make_operand(quad->result), nullptr, nullptr, quad->line);
}

Instruction*    
IopCodeDispatcher::generate_TABLESETELEM(const Quad* quad) {
    assert(quad->op == TABLESETELEM_t);
    return new Instruction(TABLESETELEM_VM, make_operand(quad->result), nullptr, nullptr, quad->line);
}