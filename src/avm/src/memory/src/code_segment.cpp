#include "../include/code_segment.h"
#include <iostream>

namespace avm
{
    namespace memory
    {
    bool   
    CodeSegment::empty() const {
        return instrs.empty();    
    }

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
        std::cout << index << std::endl;
        PRECONDITION(util::range::in_range<int>(index, 0, instrs.size() - 1));
        POSTCONDITION(instrs[index] != nullptr);
        return instrs[index];
    }

    void   
    CodeSegment::clear() {
        PRECONDITION(!empty());
        instrs.clear();
        POSTCONDITION(empty());    
    }
    }
}