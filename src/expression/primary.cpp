#include "primary.h"

ExprType 
Primary::verify_type(ExprType type) {
    PRECONDITION(   type == LIB_FUNC || type == USER_FUNC || type == VAR || 
                    type == TABLE_MAKE || type == CALL ||
                    type == CONST_NUM || type == CONST_STR || type == CONST_BOOL || type == CONST_NIL   );
    return type;
}
