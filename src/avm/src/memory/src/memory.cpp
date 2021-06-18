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

    void initialize_stack_segment() {
        if (!stack_segment.empty())
            stack_segment.clear();
    }
    
    }
}