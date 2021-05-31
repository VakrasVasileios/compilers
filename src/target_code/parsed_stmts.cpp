#include "parsed_stmts.h"

namespace target_code {
    inline bool InLoop() {
        return loop_stmts.size() != 0; 
    }

    inline bool InFuncDef() {
        return func_def_stmts.size() != 0; 
    }
} 
