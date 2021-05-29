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
    return new Instruction(A, nullptr, nullptr, nullptr, 0); //PROXY
}

Instruction*    
IopCodeDispatcher::generate_ADD(const Quad* quad) {
    assert(quad->op == ADD_t);
    return new Instruction(B, nullptr, nullptr, nullptr, 0); //PROXY
}

Instruction*
IopCodeDispatcher::generate_SUB(const Quad* quad) {
    assert(quad->op == SUB_t);
    return new Instruction(C, nullptr, nullptr, nullptr, 0); //PROXY
}

Instruction*
IopCodeDispatcher::generate_MUL(const Quad* quad) {
    assert(quad->op == MUL_t);
    return new Instruction(D, nullptr, nullptr, nullptr, 0); //PROXY
}

Instruction*
IopCodeDispatcher::generate_DIV(const Quad* quad) {
    assert(quad->op == DIV_t);
    return new Instruction(E, nullptr, nullptr, nullptr, 0); //PROXY
}

Instruction*
IopCodeDispatcher::generate_MOD(const Quad* quad) {
    assert(quad->op == MOD_t);
    return new Instruction(F, nullptr, nullptr, nullptr, 0); //PROXY
}

Instruction*
IopCodeDispatcher::generate_UMINUS(const Quad* quad) {
    assert(quad->op == UMINUS_t);
    return new Instruction(G, nullptr, nullptr, nullptr, 0); //PROXY
}

Instruction*    
IopCodeDispatcher::generate_AND(const Quad* quad) {
    assert(quad->op == AND_t);
    return new Instruction(H, nullptr, nullptr, nullptr, 0); //PROXY
}

Instruction*    
IopCodeDispatcher::generate_OR(const Quad* quad) {
    assert(quad->op == OR_t);
    return new Instruction(I, nullptr, nullptr, nullptr, 0); //PROXY
}

Instruction*    
IopCodeDispatcher::generate_NOT(const Quad* quad) {
    assert(quad->op == NOT_t);
    return new Instruction(J, nullptr, nullptr, nullptr, 0); //PROXY
}

Instruction*
IopCodeDispatcher::generate_JUMP(const Quad* quad) {
    assert(quad->op == JUMP_t);
    return new Instruction(K, nullptr, nullptr, nullptr, 0); //PROXY
}

Instruction*
IopCodeDispatcher::generate_IF_EQ(const Quad* quad) {
    assert(quad->op == IF_EQ_t);
    return new Instruction(L, nullptr, nullptr, nullptr, 0); //PROXY
}

Instruction*
IopCodeDispatcher::generate_IF_NOTEQ(const Quad* quad) {
    assert(quad->op == IF_NOTEQ_t);
    return new Instruction(M, nullptr, nullptr, nullptr, 0); //PROXY
}

Instruction*    
IopCodeDispatcher::generate_IF_LESSEQ(const Quad* quad) {
    assert(quad->op == IF_LESSEQ_t);
    return new Instruction(N, nullptr, nullptr, nullptr, 0); //PROXY
}

Instruction*
IopCodeDispatcher::generate_IF_GREATEREQ(const Quad* quad) {
    assert(quad->op == IF_GREATEREQ_t);
    return new Instruction(O, nullptr, nullptr, nullptr, 0); //PROXY
}

Instruction*
IopCodeDispatcher::generate_IF_LESS(const Quad* quad) {
    assert(quad->op == IF_LESS_t);
    return new Instruction(P, nullptr, nullptr, nullptr, 0); //PROXY
}

Instruction*    
IopCodeDispatcher::generate_IF_GREATER(const Quad* quad) {
    assert(quad->op == IF_GREATER_t);
    return new Instruction(Q, nullptr, nullptr, nullptr, 0); //PROXY
}

Instruction*    
IopCodeDispatcher::generate_CALL(const Quad* quad) {
    assert(quad->op == CALL_t);
    return new Instruction(R, nullptr, nullptr, nullptr, 0); //PROXY
}

Instruction*
IopCodeDispatcher::generate_PARAM(const Quad* quad) {
    assert(quad->op == PARAM_t);
    return new Instruction(S, nullptr, nullptr, nullptr, 0); //PROXY
}

Instruction*    
IopCodeDispatcher::generate_RETURN(const Quad* quad) {
    assert(quad->op == RET_t);
    return new Instruction(T, nullptr, nullptr, nullptr, 0); //PROXY
}

Instruction*
IopCodeDispatcher::generate_GETRETVAL(const Quad* quad) {
    assert(quad->op == GETRETVAL_t);
    return new Instruction(U, nullptr, nullptr, nullptr, 0); //PROXY
}

Instruction*
IopCodeDispatcher::generate_FUNCSTART(const Quad* quad) {
    assert(quad->op == FUNCSTART_t);
    return new Instruction(V, nullptr, nullptr, nullptr, 0); //PROXY
}

Instruction*    
IopCodeDispatcher::generate_FUNCEND(const Quad* quad) {
    assert(quad->op == FUNCEND_t);
    return new Instruction(W, nullptr, nullptr, nullptr, 0); //PROXY
}

Instruction*    
IopCodeDispatcher::generate_TABLECREATE(const Quad* quad) {
    assert(quad->op == TABLECREATE_t);
    return new Instruction(X, nullptr, nullptr, nullptr, 0); //PROXY
}

Instruction*    
IopCodeDispatcher::generate_TABLEGETELEM(const Quad* quad) {
    assert(quad->op == TABLEGETELEM_t);
    return new Instruction(Y, nullptr, nullptr, nullptr, 0); //PROXY
}

Instruction*    
IopCodeDispatcher::generate_TABLESETELEM(const Quad* quad) {
    assert(quad->op == TABLESETELEM_t);
    return new Instruction(Z, nullptr, nullptr, nullptr, 0); //PROXY
}