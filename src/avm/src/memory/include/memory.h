#ifndef MEMORY_H
#define MEMORY_H

#include "code_segment.h"
#include "stack_segment.h"
#include "constants.h"

namespace avm
{
    namespace memory
    {
    /**
     * @brief The code memory segment.
     * Here all of the target code instructions are loaded.
     * 
     */
    extern CodeSegment       code_segment;
    /**
     * @brief The main memory stack segment.
     * Here all of the 'alpha' variables are loaded.
     * 
     */
    extern StackSegment      stack_segment; 
    } 
}

#endif