#ifndef EXPRESSION_VISITOR_H
#define EXPRESSION_VISITOR_H

#include "../../../../../util/contract/contract.h"

namespace expression {
    class ArithmeticExpr;
    class AssignExpr;
    class BoolExpr;
    class Call;
    class DoubleConstant;
    class IntConstant;
    class NilConstant;
    class BoolConstant;
    class StringConstant;
    class Symbol;
    class TableMakeElems;
    class TableMakePairs;

    /**
     * @brief An interface for visiting expression
     * concrete extensions.
     * 
     */
    class ExpressionVisitor {
    public:
        /**
         * @brief Visits an arithmetic expression.
         * 
         * @param arithm_expr the arithmetic expression to be visited,
         * not null
         */
        virtual void VisitArithmeticExpression(ArithmeticExpr* arithm_expr) = 0;
        /**
         * @brief Visits an assignment expression.
         * 
         * @param assign_expr the assignment expression to be visited,
         * not null
         */
        virtual void VisitAssignExpr(AssignExpr* assign_expr) = 0;
        /**
         * @brief Visits a boolean expression
         * 
         * @param bool_expr the boolean expression to be visited,
         * not null  
         */
        virtual void VisitBoolExpr(BoolExpr* bool_expr) = 0;
        /**
         * @brief Visits a call expression.
         * 
         * @param call the call expression to be visited,
         * not null
         */
        virtual void VisitCall(Call* call) = 0;
        /**
         * @brief Visits a double constant expression.
         * 
         * @param double_const the double constant expression to be visited,
         * not null
         */
        virtual void VisitDoubleConstant(DoubleConstant* double_const) = 0;
        /**
         * @brief Visits an int constant expression.
         * 
         * @param int_const the int constant expression to be visited,
         * not null
         */
        virtual void VisitIntConstant(IntConstant* int_const) = 0;
        /**
         * @brief Visits a nill constant expression.
         * 
         * @param nil_const the nil constant expression to be visited,
         * not null
         */
        virtual void VisitNilConstant(NilConstant* nil_const) = 0;
        /**
         * @brief Visits a boolean constant expression.
         * 
         * @param bool_const the boolean constant expression to be visited,
         * not null
         */
        virtual void VisitBoolConstant(BoolConstant* bool_const) = 0;
        /**
         * @brief Visits a string constant expression.
         * 
         * @param string_const The string constant expression to be visited,
         * not null
         */
        virtual void VisitStringConstant(StringConstant* string_const) = 0;
        /**
         * @brief Visits a symbol expression.
         * 
         * @param sym the symbol expression to be visited, not null
         */
        virtual void VisitSymbol(Symbol* sym) = 0;
        /**
         * @brief Visits a table make elements expression.
         * 
         * @param table_make_elems the table make elements expression to be
         * visited, not null
         */
        virtual void VisitTableMakeElems(TableMakeElems* table_make_elems) = 0;
        /**
         * @brief Visits a table make pairs expression.
         * 
         * @param table_make_pairs the table make pairs expression to be
         * visited, not null
         */
        virtual void VisitTableMakePairs(TableMakePairs* table_make_pairs) = 0;
    };
}

#endif