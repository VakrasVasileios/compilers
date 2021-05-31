#ifndef PRIMARY_H
#define PRIMARY_H

#include "expression.h"
#include "../../util/contract/contract.h"

namespace expression {
    /**
     * @brief A primary expression.
     * 
     */
    class Primary : public Expression {
    protected:
        /**
         * @brief Constructs a new Primary object.
         * 
         * @param type the expression type of this
         * Primary object, must be LIB_FUNC, USER_FUNC,
         * VAR, TABLE_MAKE, TABLE_ITEM, CALL, CONST_NUM,
         *  CONST_BOOL, or CONST_NIL.
         */
        Primary(ExprType type) : Expression(verify_type(type)) {};
        /**
         * @brief Destroys this Primary object.
         * 
         */
        ~Primary() = default; 
    private:
        ExprType    verify_type(ExprType type);    
    };
}

#endif 
