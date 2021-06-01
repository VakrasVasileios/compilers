#include "../../include/target_code/target_code.h"

#define SYMBOL_CAST(expr)       static_cast<expression::Symbol*>(expr)
#define BOOLCONST_CAST(exr)     static_cast<expression::BoolConstant*>(expr)
#define INTCONST_CAST(exr)      static_cast<expression::IntConstant*>(expr)
#define DOUBLECONST_CAST(expr)  static_cast<expression::DoubleConstant*>(expr)
#define NILCONST_CAST(exr)      static_cast<expression::NilConstant*>(expr)
#define STRINGCONST_CAST(exr)   static_cast<expression::StringConstant*>(expr)

namespace target_code {
    virtual_machine::Vmarg* 
    IopCodeDispatcher::make_operand (expression::Expression* expr) {
        virtual_machine::Vmarg* arg = new virtual_machine::Vmarg();
        switch (expr->get_type()) {
            case expression::VAR:
            case expression::TABLE_ITEM:
            case expression::ARITHMETIC:
            case expression::BOOL:
            case expression::TABLE_MAKE: {
                arg->value = SYMBOL_CAST(expr)->get_offset();
                switch (SYMBOL_CAST(expr)->get_space()) {
                    case expression::PROGRAM_VAR:
                        arg->type = virtual_machine::GLOBAL_a;
                        break;
                    case expression::FUNCTION_LOCAL:
                        arg->type = virtual_machine::LOCAL_a;
                        break;
                    case expression::FORMAL_ARG:
                        arg->type = virtual_machine::FORMAL_a;
                        break;
                    default:
                        assert(false);
                }
            }
            case expression::CONST_BOOL: {
                arg->type = virtual_machine::BOOL_a;
                arg->value = BOOLCONST_CAST(expr)->get_value();
                break;
            }
            case expression::CONST_STR: {
                arg->type = virtual_machine::STRING_a;
                arg->value = virtual_machine::ProgramConsts:: GetInstance().InsertString(STRINGCONST_CAST(expr)->get_value());
                break;
            }
            case expression::CONST_INT: {
                arg->type = virtual_machine::NUMBER_a;
                arg->value = virtual_machine::ProgramConsts:: GetInstance().InsertNumber(INTCONST_CAST(expr)->get_value());
                break;
            }
            case expression::CONST_DOUBLE: {
                arg->type = virtual_machine::NUMBER_a;
                arg->value = virtual_machine::ProgramConsts:: GetInstance().InsertNumber(DOUBLECONST_CAST(expr)->get_value());
                break;
            }
            case expression::CONST_NIL:
                arg->type = virtual_machine::NIL_a;
                break;
            case expression::LIB_FUNC: {
                arg->type = virtual_machine::LIBFUNC_a;
                arg->value = virtual_machine::ProgramConsts:: GetInstance().InsertLibFunc(SYMBOL_CAST(expr)->get_id());
            }
            case expression::USER_FUNC: {
                arg->type = virtual_machine::USERFUNC_a;
                // TODO: Pass taddress of user function to arg.val
            }
            default:
                assert(false);
        }

        return arg;
    }

    virtual_machine::Vmarg*              
    IopCodeDispatcher::make_numberoperand (expression::Expression* expr) {
        
    }

    virtual_machine::Vmarg*
    IopCodeDispatcher::make_booloperand (expression::Expression* expr) {
        
    }

    virtual_machine::Vmarg*
    IopCodeDispatcher::make_retvaloperand (expression::Expression* expr) {
        
    }
    /* 
    The dispatch takes place because the generate functions index in this
    array matches each respective opcode enum value, from which each  vm
    instruction is generated.                               
    */
    void    
    IopCodeDispatcher::Generate(){
        for (auto quad : intermediate_code::quads)
            (this->*generators[quad->op])(quad);
    }

    void        
    IopCodeDispatcher::generate(virtual_machine::Vmopcode op,  intermediate_code::Quad* quad) {
        Emit(new virtual_machine::Instruction(NextInstructionLabel(), op, make_operand(quad->result),
            make_operand(quad->arg1), make_operand(quad->arg2), quad->line));
        quad->taddress = NextInstructionLabel();
    }

    void 
    IopCodeDispatcher::generate_relational(virtual_machine::Vmopcode op, intermediate_code::Quad* quad) {
        // TODO
    }

    void    
    IopCodeDispatcher::generate_ASSIGN(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::ASSIGN_t);
        generate(virtual_machine::ASSIGN_VM, quad);
    }

    void    
    IopCodeDispatcher::generate_ADD(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::ADD_t);
        generate(virtual_machine::ADD_VM, quad);
    }

    void
    IopCodeDispatcher::generate_SUB(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::SUB_t);
        generate(virtual_machine::SUB_VM, quad);
    }

    void
    IopCodeDispatcher::generate_MUL(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::MUL_t);
        generate(virtual_machine::MUL_VM, quad);
    }

    void
    IopCodeDispatcher::generate_DIV(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::DIV_t);
        generate(virtual_machine::DIV_VM, quad);
    }

    void
    IopCodeDispatcher::generate_MOD(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::MOD_t);
        generate(virtual_machine::MOD_VM, quad);
    }

    void
    IopCodeDispatcher::generate_UMINUS(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::UMINUS_t);
        Emit(new virtual_machine::Instruction(NextInstructionLabel(), virtual_machine::MUL_VM, make_operand(quad->result),
            make_operand(quad->arg1), make_operand(new expression::IntConstant(-1)), quad->line));
        quad->taddress = NextInstructionLabel();
    }

    void
    IopCodeDispatcher::generate_JUMP(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::JUMP_t);
        generate_relational(virtual_machine::JUMP_VM, quad);
    }

    void
    IopCodeDispatcher::generate_IF_EQ(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::IF_EQ_t);
        generate_relational(virtual_machine::JEQ_VM, quad);
    }

    void
    IopCodeDispatcher::generate_IF_NOTEQ(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::IF_NOTEQ_t);
        generate_relational(virtual_machine::JNE_VM, quad);
    }

    void    
    IopCodeDispatcher::generate_IF_LESSEQ(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::IF_LESSEQ_t);
        generate_relational(virtual_machine::JLE_VM, quad);
    }

    void
    IopCodeDispatcher::generate_IF_GREATEREQ(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::IF_GREATEREQ_t);
        generate_relational(virtual_machine::JGE_VM, quad);
    }

    void
    IopCodeDispatcher::generate_IF_LESS(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::IF_LESS_t);
        generate_relational(virtual_machine::JLT_VM, quad);
    }

    void    
    IopCodeDispatcher::generate_IF_GREATER(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::IF_GREATER_t);
        generate_relational(virtual_machine::JGT_VM, quad);
    }

    void    
    IopCodeDispatcher::generate_CALL(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::CALL_t);
        Emit(new virtual_machine::Instruction(NextInstructionLabel(),
             virtual_machine::CALLFUNC_VM, make_operand(quad->result), nullptr, nullptr, quad->line));
        quad->taddress = NextInstructionLabel();
    }

    void
    IopCodeDispatcher::generate_PARAM(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::PARAM_t);
        Emit(new virtual_machine::Instruction(NextInstructionLabel(),
             virtual_machine::PUSHARG_VM, make_operand(quad->result), nullptr, nullptr, quad->line));
        quad->taddress = NextInstructionLabel();
    }

    void    
    IopCodeDispatcher::generate_RETURN(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::RET_t);
    }

    void
    IopCodeDispatcher::generate_GETRETVAL(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::GETRETVAL_t);
        Emit(new virtual_machine::Instruction(NextInstructionLabel(), virtual_machine::ASSIGN_VM,
             make_operand(quad->result), make_retvaloperand(quad->arg1), nullptr, quad->line));
        quad->taddress = NextInstructionLabel();
    }

    void
    IopCodeDispatcher::generate_FUNCSTART(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::FUNCSTART_t);
    }

    void    
    IopCodeDispatcher::generate_FUNCEND(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::FUNCEND_t);
    }

    void    
    IopCodeDispatcher::generate_TABLECREATE(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::TABLECREATE_t);
        generate(virtual_machine::NEWTABLE_VM, quad);
    }

    void    
    IopCodeDispatcher::generate_TABLEGETELEM(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::TABLEGETELEM_t);
        generate(virtual_machine::TABLEGETELEM_VM, quad);
    }

    void    
    IopCodeDispatcher::generate_TABLESETELEM(intermediate_code::Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == intermediate_code::TABLESETELEM_t);
        generate(virtual_machine::TABLESETELEM_VM, quad);
    }
}