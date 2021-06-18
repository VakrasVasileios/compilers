#ifndef CODE_SEGMENT_H
#define CODE_SEGMENT_H

#include <vector>
#include "../../../../../util/contract/include/contract.h"
#include "../../../../../util/range/include/range.h"
#include "../../../../target_code/include/instruction.h"

namespace avm
{
    namespace memory
    {
    /**
     * @brief The avm memory segment that contains 
     * the instructions.
     * 
     */
    class CodeSegment final {
    public:
        /**
         * @brief Constructs a new CodeSegment object.
         * 
         */
        CodeSegment() = default;
        /**
         * @brief Destroys this CodeSegment object.
         * 
         */
        ~CodeSegment() = default;
        /**
         * @brief Checks wether this CodeSegment is empty.
         * 
         * @return wether this CodeSegment is empty 
         */
        bool   empty() const;
        /**
         * @brief Pushes a target code instruction to this
         * CodeSegment.
         * 
         * @param _instr the target code instruction to be
         * pushed to this CodeSegment, not null
         */
        void  push(target_code::Instruction* _instr);
        /**
         * @brief Returns an O(1) access to an inserted 
         * target code instruction.
         * 
         * @param index the index of the instruction, must be
         * in a valid range
         * 
         * @return an O(1) access to an inserted 
         * target code instruction, not null
         */
        target_code::Instruction*  &operator[](int index);
        /**
         * @brief Clears this non empty
         * CodeSegment.
         * 
         */
        void   clear();   
    private:
        std::vector<target_code::Instruction*> instrs;    
    };
    }
}

#endif