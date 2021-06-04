#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>
#include "../../../../../util/contract/contract.h"

/**
 * @brief Alpha expression namespace.
 * 
 */
namespace expression {
    /**
     * @brief The type of an expression.
     * 
    **/ 
    enum ExprType {
        ASSIGN,
        BOOL,
        ARITHMETIC,
        
        LIB_FUNC,
        USER_FUNC,
        VAR,
        CONST_INT,
        CONST_DOUBLE,
        CONST_BOOL,
        CONST_STR,
        CONST_NIL,
        CALL,
        TABLE_ITEM,
        TABLE_MAKE
    };

    /**
     * @brief An expression abstraction.
     * 
    **/ 
    class Expression {
    public:
        /**
         * @brief Returns a read access to this Expression type.
         * 
         * @return a read access to this Expression type.
        **/ 
        ExprType                get_type() const;
    
        friend std::ostream&    operator << (std::ostream& os, const Expression* expression);
        /**
         * @brief Returns a read access to this Expression as a 
         * string.
         * 
         * @return a read access to this Expression as a 
         * string.
         */
        virtual std::string     to_string() const = 0;
        virtual unsigned int    to_unsigned() const = 0;
    protected:
        Expression(ExprType _type) : type(_type) {};
        ~Expression() = default;
        /**
         * @brief Logs this Expression.
         * 
         * @param os the ostream to output this Expression.
         * 
        **/ 
        virtual std::ostream&   LogExpression(std::ostream& os) const = 0;
    private:
        ExprType type;
    };
    /**
     * @brief Checks wether an expression is a 
     * library function.
     * 
     * @param expr the expression to be checked, not null
     * 
     * @return wether an expression is a 
     * library function
     */
    bool                        IsLibraryFunction(Expression* expr);
    /**
     * @brief Checks wether an expression is a 
     * user function.
     * 
     * @param expr the expression to be checked, not null
     * 
     * @return wether an expression is a 
     * user function
     */
    bool                        IsUserFunction(Expression* expr);
    /**
     * @brief Checks wether an expression is a 
     * variable.
     * 
     * @param expr the expression to be checked, not null
     * 
     * @return wether an expression is a 
     * variable
     */
    bool                        IsVariable(Expression* expr);
    /**
     * @brief Checks wether an expression is a 
     * constant boolean.
     * 
     * @param expr the expression to be checked, not null
     * 
     * @return wether an expression is a 
     * constant boolean.
     */
    bool                        IsConstBool(Expression* expr);
    /**
     * @brief Checks wether an expression is a 
     * constant string.
     * 
     * @param expr the expression to be checked, not null
     * 
     * @return wether an expression is a 
     * constant string.
     */
    bool                        IsConstString(Expression* expr);
    /**
     * @brief Checks wether an expression is a 
     * table make.
     * 
     * @param expr the expression to be checked, not null
     * 
     * @return wether an expression is a 
     * table make.
     */
    bool                        IsTableMake(Expression* expr);
}

#endif
