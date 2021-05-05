#include "../../include/expression/constant.h"
#include <assert.h>
#include <iostream>

// template<typename typ>
// typ 
// Constant:: get_value(void) const {
//     switch (get_type()) {
//         case CONST_NUM:
//             return num;
//         case CONST_STR:
//             return str;
//         case CONST_BOOL:
//             return boolVal;
//         case NIL_t:
//             return _NIL;
//         default:
//             assert(false);
//     }
// }

void
Constant:: LogExpr(std::ostream& output) {
    switch (get_type()) {
        case CONST_NUM:
            output << num;
            break;
        case CONST_STR:
            output << str;
            break;
        case CONST_BOOL:
            output << boolVal;
            break;
        case NIL_t:
            output << "nil";
            break;
        default:
            assert(false);
    }
}