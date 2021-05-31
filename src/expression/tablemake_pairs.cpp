#include "../../include/expression/tablemake_pairs.h"

namespace expression {
    Indexed* 
    TableMakePairs::get_indexed() const {
        return indexed;
    }

    Indexed*    
    TableMakePairs::verify_indexed(Indexed* indexed) {
        PRECONDITION(indexed != nullptr);
        return indexed;
    }
}