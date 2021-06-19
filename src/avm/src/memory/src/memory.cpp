#include "../include/memory.h"

namespace avm
{
    namespace memory
    {
    CodeSegment     code_segment;
    StackSegment    stack_segment;

    void initialize_code_segment() {
        if (!code_segment.empty())
            code_segment.clear();
    }

    void initialize_stack_segment(unsigned int global_off) {
        if (!stack_segment.empty())
            stack_segment.clear();

        for (int i = 0; i < global_off; i++) {
                memory::stack_segment.push(new memcell::UndefMemcell());
            }
    }
    
    }
}