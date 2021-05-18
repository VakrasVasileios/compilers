#include "../include/quad.h"
#include <assert.h>

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

void PatchJumpQuad(Quad* jump_quad, const unsigned int label) {
    jump_quad->result = new IntConstant(label);
}

void PatchBranchQuad(Quad* branch_quad, const unsigned int label) {
    branch_quad->arg2 = new IntConstant(label);
}