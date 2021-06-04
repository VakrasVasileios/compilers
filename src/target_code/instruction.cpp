#include "../../include/target_code/instruction.h"

namespace target_code {
    std::ostream& 
    operator<<(std::ostream &os, const instruction rhs) {
        os  << rhs.label << ": " << VmopcodeToString(rhs.opcode);
            if (rhs.result != nullptr)
                os << " " << *rhs.result;
            if (rhs.arg1 != nullptr)    
                os << " " << *rhs.arg1;
            if (rhs.arg2 != nullptr)    
                os << " " << *rhs.arg2;
            os << "[line " << rhs.src_line << "]" << std::endl;
        return os;
    }
}