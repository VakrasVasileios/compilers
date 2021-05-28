#include "opcode_dispatcher.h"

Instruction*
IopCodeDispatcher::Generate(const Quad* quad){
    PRECONDITION(quad != nullptr);
    return (this->*generators[quad->op])(quad);
}

Instruction*
IopCodeDispatcher::generate_ADD(const Quad* quad) {
    assert(quad->op == ADD_t);
}

Instruction*
IopCodeDispatcher::generate_SUB(const Quad* quad) {
    assert(quad->op == SUB_t);
}

Instruction*
IopCodeDispatcher::generate_MUL(const Quad* quad) {
    assert(quad->op == MUL_t);
}

Instruction*
IopCodeDispatcher::generate_DIV(const Quad* quad) {
    assert(quad->op == DIV_t);
}

Instruction*
IopCodeDispatcher::generate_MOD(const Quad* quad) {
    assert(quad->op == MOD_t);
}

Instruction*
IopCodeDispatcher::generate_TABLECREATE(const Quad* quad) {
    assert(quad->op == TABLECREATE_t);
}

Instruction*
IopCodeDispatcher::generate_TABLEGETELEM(const Quad* quad) {
    assert(quad->op == TABLEGETELEM_t);
}

Instruction*
IopCodeDispatcher::generate_TABLESETELEM(const Quad* quad) {
    assert(quad->op == TABLESETELEM_t);
}

Instruction*
IopCodeDispatcher::generate_ASSIGN(const Quad* quad) {
    assert(quad->op == ASSIGN_t);
}

Instruction*
IopCodeDispatcher::generate_JUMP(const Quad* quad) {
    assert(quad->op == JUMP_t);
}

Instruction*
IopCodeDispatcher::generate_IF_EQ(const Quad* quad) {
    assert(quad->op == IF_EQ_t);
}

Instruction*
IopCodeDispatcher::generate_IF_GREATER(const Quad* quad) {
    assert(quad->op == IF_GREATER_t);
}

Instruction*
IopCodeDispatcher::generate_IF_GREATEREQ(const Quad* quad) {
    assert(quad->op == IF_GREATEREQ_t);
}

Instruction*
IopCodeDispatcher::generate_IF_LESS(const Quad* quad) {
    assert(quad->op == IF_LESS_t);
}

Instruction*
IopCodeDispatcher::generate_IF_LESSEQ(const Quad* quad) {
    assert(quad->op == IF_LESSEQ_t);
}

Instruction*
IopCodeDispatcher::generate_NOT(const Quad* quad) {
    assert(quad->op == NOT_t);
}

Instruction*
IopCodeDispatcher::generate_OR(const Quad* quad) {
    assert(quad->op == OR_t);
}

Instruction*
IopCodeDispatcher::generate_AND(const Quad* quad) {
    assert(quad->op == AND_t);
}

Instruction*
IopCodeDispatcher::generate_PARAM(const Quad* quad) {
    assert(quad->op == PARAM_t);
}

Instruction*
IopCodeDispatcher::generate_CALL(const Quad* quad) {
    assert(quad->op == CALL_t);
}

Instruction*
IopCodeDispatcher::generate_GETRETVAL(const Quad* quad) {
    assert(quad->op == GETRETVAL_t);
}

Instruction*
IopCodeDispatcher::generate_FUNCSTART(const Quad* quad) {
    assert(quad->op == FUNCSTART_t);
}

Instruction*
IopCodeDispatcher::generate_RETURN(const Quad* quad) {
    assert(quad->op == RET_t);
}

Instruction*
IopCodeDispatcher::generate_FUNCEND(const Quad* quad) {
    assert(quad->op == FUNCEND_t);
}
