#ifndef MAKE_OPERAND_VISITOR_H
#define MAKE_OPERAND_VISITOR_H

#include "expression/include/expression.h"
#include "program_consts.h"
#include "vm_arg.h"

namespace target_code {
    /**
     * @brief A factory expression visitor interface implementation
     * for creating a virtual machine argument operand.
     * 
     */
    class MakeOperandVisitor final : public expression::ExpressionVisitor {
    public:
        /**
         * @brief Constructs a new MakeOperandVisitor object.
         * 
         */
        MakeOperandVisitor() = default;
        /**
         * @brief Destroys this MakeOperandVisitor object.
         * 
         */
        ~MakeOperandVisitor() = default;
        
        void    VisitArithmeticExpression(
                    expression::ArithmeticExpr* arithm_expr) override;
        void    VisitAssignExpr(expression::AssignExpr* assign_expr) override;
        void    VisitBoolExpr(expression::BoolExpr* bool_expr) override;
        void    VisitCall(expression::Call* call) override;
        void    VisitDoubleConstant(
                    expression::DoubleConstant* double_const) override;
        void    VisitIntConstant(expression::IntConstant* int_const) override;
        void    VisitNilConstant(expression::NilConstant* nil_const) override;
        void    VisitBoolConstant(
                    expression::BoolConstant* bool_const) override;
        void    VisitStringConstant(expression::StringConstant* string_const)
                    override;
        void    VisitSymbol(expression::Symbol* sym) override;
        void    VisitTableMakeElems(
                    expression::TableMakeElems* table_make_elems) override;
        void    VisitTableMakePairs(
                    expression::TableMakePairs* table_make_pairs) override;
        /**
         * @brief Returns a read access to the result
         * virtual machine argument operand of this
         * MakeOperandVisitor.
         * 
         * @return a read access to the result
         * virtual machine argument operand of this
         * MakeOperandVisitor, not null
         */
        Vmarg*  get_result() const;
    private:
        Vmarg* result;
    };
}

#endif