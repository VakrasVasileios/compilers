#ifndef AVM_CODE_SEGMENT_H
#define AVM_CODE_SEGMENT_H

#include <vector>
#include "../../../../../util/contract/include/contract.h"
#include "../../../../../util/range/include/range.h"
#include "../../../../target_code/include/instruction.h"

namespace avm
{
    namespace code_segment
    {
    /**
     * @brief The avm memory segment that contains 
     * the instructions.
     * 
     */
    class AvmCodeSegment final {
    public:
        /**
         * @brief Constructs a new AvmCodeSegment object.
         * 
         */
        AvmCodeSegment() = default;
        /**
         * @brief Destroys this AvmCodeSegment object.
         * 
         */
        ~AvmCodeSegment() = default;
        /**
         * @brief Pushes a target code instruction to this
         * AvmCodeSegment.
         * 
         * @param _instr the target code instruction to be
         * pushed to this AvmCodeSegment, not null
         */
        void  push(const target_code::Instruction* _instr);
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
        const target_code::Instruction* &operator[](int index);
    private:
        std::vector<const target_code::Instruction*> instrs;    
    };
    }
}

#endif