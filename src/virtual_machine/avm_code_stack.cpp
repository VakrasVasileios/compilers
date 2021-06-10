#include "avm_code_stack.h"

namespace virtual_machine
{

void 
AvmCodeStack::push(const target_code::Instruction* _instr) {
    PRECONDITION(_instr != nullptr);
    instrs.push_back(_instr);
}

const target_code::Instruction*&
AvmCodeStack::operator[](int index) {
    PRECONDITION(util::in_range<int>(index, 0, instrs.size() - 1));
    POSTCONDITION(instrs[index] != nullptr);
    return instrs[index];
}

}