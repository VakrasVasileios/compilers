#include "../../include/expression/constant.h"
#include <assert.h>

template<typename typ> typ
Constant:: get_value(void) const {
    switch (type) {
    case INT_T:
        return intNum;
    case DOUBLE_T:
        return doubleNum;
    case STRING_T:
        return str;
    case BOOL_T:
        return boolVal;
    case NIL_T:
        return nullptr;
    default:
        assert(false);
    }
}