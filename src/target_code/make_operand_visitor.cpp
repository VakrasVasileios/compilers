#include "make_operand_visitor.h"
#include <assert.h>

namespace target_code {
    void   
    MakeOperandVisitor::VisitArithmeticExpression(expression::ArithmeticExpr* arithm_expr) {
        arithm_expr->get_result()->Accept(this);
    }

    void    
    MakeOperandVisitor::VisitAssignExpr(expression::AssignExpr* assign_expr) {
       assign_expr->get_result()->Accept(this);
    }

    void    
    MakeOperandVisitor::VisitBoolExpr(expression::BoolExpr* bool_expr) {
        bool_expr->get_result()->Accept(this);
    }

    void    
    MakeOperandVisitor::VisitCall(expression::Call* call) {
        VisitSymbol(call->get_called_symbol());
    }

    void    
    MakeOperandVisitor::VisitDoubleConstant(expression::DoubleConstant* double_const) {
        PRECONDITION(double_const != nullptr);
        result = new Vmarg(NUMBER_a, ProgramConsts:: GetInstance().InsertNumber(double_const->get_value()));
    }

    void    
    MakeOperandVisitor::VisitIntConstant(expression::IntConstant* int_const) {
        PRECONDITION(int_const != nullptr);
        result = new Vmarg(NUMBER_a, ProgramConsts:: GetInstance().InsertNumber(int_const->get_value()));
    }

    void    
    MakeOperandVisitor::VisitNilConstant(expression::NilConstant* nil_const) {
        PRECONDITION(nil_const != nullptr);
        result = new Vmarg(NIL_a);
    }

    void    
    MakeOperandVisitor::VisitBoolConstant(expression::BoolConstant* bool_const) {
        PRECONDITION(bool_const != nullptr);
        result = new Vmarg(BOOL_a, bool_const->get_value());
    }

    void    
    MakeOperandVisitor::VisitStringConstant(expression::StringConstant* string_const) {
        PRECONDITION(string_const != nullptr);
        result = new Vmarg(STRING_a, ProgramConsts:: GetInstance().InsertString(string_const->get_value()));
    }

    void    
    MakeOperandVisitor::VisitSymbol(expression::Symbol* sym) {
        PRECONDITION(sym != nullptr);
        Vmarg* vm_arg;
        switch (sym->get_type()) {
            case expression::USER_FUNC:
                std::cout << "Balloons " << sym->get_id() << std::endl;
                vm_arg = new Vmarg(USERFUNC_a, ProgramConsts::GetInstance().InsertUserFunc(sym));
                break;
            case expression::LIB_FUNC:
                vm_arg = new Vmarg(LIBFUNC_a, ProgramConsts::GetInstance().InsertLibFunc(sym->get_id()));
                break;
            default:        
                vm_arg = new Vmarg(sym->get_offset());
                switch (sym->get_space()) {
                    case expression::PROGRAM_VAR:
                        vm_arg->type = GLOBAL_a;
                        break;
                    case expression::FUNCTION_LOCAL:
                        vm_arg->type = LOCAL_a;
                        break;    
                    case expression::FORMAL_ARG:
                        vm_arg->type = FORMAL_a;
                        break;   
                    default:
                        assert(false);     
                }
        }
        result = vm_arg;
    }

    void    
    MakeOperandVisitor::VisitTableMakeElems(expression::TableMakeElems* table_make_elems) {
        VisitSymbol(table_make_elems->get_table());
    }

    void    
    MakeOperandVisitor::VisitTableMakePairs(expression::TableMakePairs* table_make_pairs) {
        VisitSymbol(table_make_pairs->get_table());
    }

    Vmarg*  
    MakeOperandVisitor::get_result() const {
        return result;
    }

}