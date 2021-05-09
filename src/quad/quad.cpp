#include "../../include/quad/quad.h"
#include <assert.h>

std::ostream&
operator<<(std::ostream& os, const quad _quad) {
    os << std::to_string(_quad.label) << ":   " << OpCodeToString(_quad.op) << " ";
    if(_quad.arg1 != nullptr)
        os << _quad.arg1 << "";
    if (_quad.arg2 != nullptr)
        os << _quad.arg2 << " ";
    os << _quad.result << " [line " << std::to_string(_quad.line) << "]";

    return os;
}