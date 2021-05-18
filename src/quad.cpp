#include "../include/quad.h"

std::ostream&
operator<<(std::ostream& os, const quad _quad) {
    os << std::to_string(_quad.label) << ":   " << IopcodeToString(_quad.op) << " ";
    if (_quad.result != nullptr)
        os << _quad.result << " ";
    if(_quad.arg1 != nullptr)
        os << _quad.arg1 << " ";
    if (_quad.arg2 != nullptr)
        os << _quad.arg2 << " ";
    os << "[line " << std::to_string(_quad.line) << "]";

    return os;
}

bool IsJumpQuad(Quad* quad) {
    PRECONDITION(quad != nullptr);

    return quad->op == JUMP_t;
}

bool IsBranchQuad(Quad* quad) {
    PRECONDITION(quad != nullptr);
    auto op = quad->op;

    return  op == IF_EQ_t || op == IF_NOTEQ_t || op == IF_LESSEQ_t
            || op == IF_GREATEREQ_t || op == IF_LESS_t || op == IF_GREATER_t;
}

void PatchJumpQuad(Quad* jump_quad, const unsigned int label) {
    PRECONDITION(jump_quad != nullptr);
    PRECONDITION(IsJumpQuad(jump_quad));
    PRECONDITION(label >= 0);
    jump_quad->result = new IntConstant(label);
}

void PatchBranchQuad(Quad* branch_quad, const unsigned int label) {
    PRECONDITION(branch_quad != nullptr);
    PRECONDITION(IsBranchQuad(branch_quad));
    PRECONDITION(label >= 0);
    branch_quad->arg2 = new IntConstant(label);
}