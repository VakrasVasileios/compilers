#include "../include/quad.h"
#include <assert.h>

void
Quad:: PatchJumpQuad(const unsigned int label) {
    result = new IntConstant(label);
}

void 
Quad:: PatchBranchQuad(const unsigned int label) {
    arg2 = new IntConstant(label);
}

std::ostream&
operator<<(std::ostream& os, const quad _quad) {
    os << std::to_string(_quad.label) << ":   " << OpCodeToString(_quad.op) << " ";
    if (_quad.result != nullptr)
        os << _quad.result << " ";
    if(_quad.arg1 != nullptr)
        os << _quad.arg1 << " ";
    if (_quad.arg2 != nullptr)
        os << _quad.arg2 << " ";
    os << "[line " << std::to_string(_quad.line) << "]";

    return os;
}