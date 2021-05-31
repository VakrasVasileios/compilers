#include "opcode_dispatcher.h"

#define SYMBOL_CAST(expr)       static_cast<Symbol*>(expr)
#define BOOLCONST_CAST(exr)     static_cast<BoolConstant*>(expr)
#define INTCONST_CAST(exr)      static_cast<IntConstant*>(expr)
#define DOUBLECONST_CAST(expr)  static_cast<DoubleConstant*>(expr)
#define NILCONST_CAST(exr)      static_cast<NilConstant*>(expr)
#define STRINGCONST_CAST(exr)   static_cast<StringConstant*>(expr)

namespace target_code {
    Vmarg* 
    IopCodeDispatcher::make_operand (Expression* expr) {
        Vmarg* arg = new Vmarg();
        switch (expr->get_type()) {
            case VAR:
            case TABLE_ITEM:
            case ARITHMETIC:
            case BOOL:
            case TABLE_MAKE: {
                arg->value = SYMBOL_CAST(expr)->get_offset();
                switch (SYMBOL_CAST(expr)->get_space()) {
                    case PROGRAM_VAR:
                        arg->type = GLOBAL_a;
                        break;
                    case FUNCTION_LOCAL:
                        arg->type = LOCAL_a;
                        break;
                    case FORMAL_ARG:
                        arg->type = FORMAL_a;
                        break;
                    default:
                        assert(false);
                }
            }
            case CONST_BOOL: {
                arg->type = BOOL_a;
                arg->value = BOOLCONST_CAST(expr)->get_value();
                break;
            }
            case CONST_STR: {
                arg->type = STRING_a;
                arg->value = ProgramConsts:: GetInstance().InsertString(STRINGCONST_CAST(expr)->get_value());
                break;
            }
            case CONST_INT: {
                arg->type = NUMBER_a;
                arg->value = ProgramConsts:: GetInstance().InsertNumber(INTCONST_CAST(expr)->get_value());
                break;
            }
            case CONST_DOUBLE: {
                arg->type = NUMBER_a;
                arg->value = ProgramConsts:: GetInstance().InsertNumber(DOUBLECONST_CAST(expr)->get_value());
                break;
            }
            case CONST_NIL:
                arg->type = NIL_a;
                break;
            case LIB_FUNC: {
                arg->type = LIBFUNC_a;
                arg->value = ProgramConsts:: GetInstance().InsertLibFunc(SYMBOL_CAST(expr)->get_id());
            }
            case USER_FUNC: {
                arg->type = USERFUNC_a;
                // TODO: Pass taddress of user function to arg.val
            }
            default:
                assert(false);
        }

        return arg;
    }

    Vmarg*              
    IopCodeDispatcher::make_numberoperand (Expression* expr) {
        
    }

    Vmarg*
    IopCodeDispatcher::make_booloperand (Expression* expr) {
        
    }

    Vmarg*
    IopCodeDispatcher::make_retvaloperand (Expression* expr) {
        
    }
    /* 
    The dispatch takes place because the generate functions index in this
    array matches each respective opcode enum value, from which each  vm
    instruction is generated.                               
    */
    void    
    IopCodeDispatcher::Generate(std::vector<Quad*> quads){
        for (auto quad : quads)
            (this->*generators[quad->op])(quad);
    }

    void        
    IopCodeDispatcher::generate(Vmopcode op,  Quad* quad) {
        Emit(new Instruction(op, make_operand(quad->result), make_operand(quad->arg1), make_operand(quad->arg2), quad->line));
        quad->taddress = NextInstructionLabel();
    }

    void 
    IopCodeDispatcher::generate_relational(Vmopcode op, Quad* quad) {
        // TODO
    }

    void    
    IopCodeDispatcher::generate_ASSIGN(Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == ASSIGN_t);
        generate(ASSIGN_VM, quad);
    }

    void    
    IopCodeDispatcher::generate_ADD(Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == ADD_t);
        generate(ADD_VM, quad);
    }

    void
    IopCodeDispatcher::generate_SUB(Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == SUB_t);
        generate(SUB_VM, quad);
    }

    void
    IopCodeDispatcher::generate_MUL(Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == MUL_t);
        generate(MUL_VM, quad);
    }

    void
    IopCodeDispatcher::generate_DIV(Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == DIV_t);
        generate(DIV_VM, quad);
    }

    void
    IopCodeDispatcher::generate_MOD(Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == MOD_t);
        generate(MOD_VM, quad);
    }

    void
    IopCodeDispatcher::generate_UMINUS(Quad* quad) {
        assert(quad != nullptr);
        assert(quad != nullptr);
        assert(quad->op == UMINUS_t);
        Emit(new Instruction(MUL_VM, make_operand(quad->result), make_operand(quad->arg1), make_operand(new IntConstant(-1)), quad->line));
        quad->taddress = NextInstructionLabel();
    }

    void
    IopCodeDispatcher::generate_JUMP(Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == JUMP_t);
        generate_relational(JUMP_VM, quad);
    }

    void
    IopCodeDispatcher::generate_IF_EQ(Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == IF_EQ_t);
        generate_relational(JEQ_VM, quad);
    }

    void
    IopCodeDispatcher::generate_IF_NOTEQ(Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == IF_NOTEQ_t);
        generate_relational(JNE_VM, quad);
    }

    void    
    IopCodeDispatcher::generate_IF_LESSEQ(Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == IF_LESSEQ_t);
        generate_relational(JLE_VM, quad);
    }

    void
    IopCodeDispatcher::generate_IF_GREATEREQ(Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == IF_GREATEREQ_t);
        generate_relational(JGE_VM, quad);
    }

    void
    IopCodeDispatcher::generate_IF_LESS(Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == IF_LESS_t);
        generate_relational(JLT_VM, quad);
    }

    void    
    IopCodeDispatcher::generate_IF_GREATER(Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == IF_GREATER_t);
        generate_relational(JGT_VM, quad);
    }

    void    
    IopCodeDispatcher::generate_CALL(Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == CALL_t);
        Emit(new Instruction(CALLFUNC_VM, make_operand(quad->result), nullptr, nullptr, quad->line));
        quad->taddress = NextInstructionLabel();
    }

    void
    IopCodeDispatcher::generate_PARAM(Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == PARAM_t);
        Emit(new Instruction(PUSHARG_VM, make_operand(quad->result), nullptr, nullptr, quad->line));
        quad->taddress = NextInstructionLabel();
    }

    void    
    IopCodeDispatcher::generate_RETURN(Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == RET_t);
    }

    void
    IopCodeDispatcher::generate_GETRETVAL(Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == GETRETVAL_t);
        Emit(new Instruction(ASSIGN_VM, make_operand(quad->result), make_retvaloperand(quad->arg1), nullptr, quad->line));
        quad->taddress = NextInstructionLabel();
    }

    void
    IopCodeDispatcher::generate_FUNCSTART(Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == FUNCSTART_t);
    }

    void    
    IopCodeDispatcher::generate_FUNCEND(Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == FUNCEND_t);
    }

    void    
    IopCodeDispatcher::generate_TABLECREATE(Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == TABLECREATE_t);
        generate(NEWTABLE_VM, quad);
    }

    void    
    IopCodeDispatcher::generate_TABLEGETELEM(Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == TABLEGETELEM_t);
        generate(TABLEGETELEM_VM, quad);
    }

    void    
    IopCodeDispatcher::generate_TABLESETELEM(Quad* quad) {
        assert(quad != nullptr);
        assert(quad->op == TABLESETELEM_t);
        generate(TABLESETELEM_VM, quad);
    }
}