#include "../include/code_segment.h"

namespace avm
{
    namespace memory
    {
    void 
    CodeSegment::push(target_code::Instruction* _instr) {
        PRECONDITION(_instr != nullptr);
        auto prev_size = instrs.size();
        instrs.push_back(_instr);
        auto new_size = instrs.size();
        POSTCONDITION(new_size == prev_size + 1);
    }

    target_code::Instruction*&
    CodeSegment::operator[](int index) {
        PRECONDITION(util::range::in_range<int>(index, 0, instrs.size() - 1));
        POSTCONDITION(instrs[index] != nullptr);
        return instrs[index];
    }
    }
}