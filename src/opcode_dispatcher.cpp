#include "opcode_dispatcher.h"

#define SYMBOL_CAST(expr)       static_cast<const Symbol*>(expr)
#define BOOLCONST_CAST(exr)     static_cast<const BoolConstant*>(expr)
#define INTCONST_CAST(exr)      static_cast<const IntConstant*>(expr)
#define DOUBLECONST_CAST(expr)  static_cast<const DoubleConstant*>(expr)
#define NILCONST_CAST(exr)      static_cast<const NilConstant*>(expr)
#define STRINGCONST_CAST(exr)   static_cast<const StringConstant*>(expr)

Vmarg*
make_operand (const Expression* expr) {
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

/* 
The dispatch takes place because the generate functions index in this
array matches each respective opcode enum value, from which each  vm
instruction is generated.                               
*/
Instruction*    
IopCodeDispatcher::Generate(const Quad* quad){
    PRECONDITION(quad != nullptr);
    return (this->*generators[quad->op])(quad);
}

Instruction*    
IopCodeDispatcher::generate_ASSIGN(const Quad* quad) {
    assert(quad->op == ASSIGN_t);
    return new Instruction(ASSIGN_VM, make_operand(quad->result), make_operand(quad->arg1), nullptr, quad->line);
}

Instruction*    
IopCodeDispatcher::generate_ADD(const Quad* quad) {
    assert(quad->op == ADD_t);
    return new Instruction(ADD_VM, make_operand(quad->result), make_operand(quad->arg1), make_operand(quad->arg2), quad->line);
}

Instruction*
IopCodeDispatcher::generate_SUB(const Quad* quad) {
    assert(quad->op == SUB_t);
    return new Instruction(SUB_VM, make_operand(quad->result), make_operand(quad->arg1), make_operand(quad->arg2), quad->line);
}

Instruction*
IopCodeDispatcher::generate_MUL(const Quad* quad) {
    assert(quad->op == MUL_t);
    return new Instruction(MUL_VM, make_operand(quad->result), make_operand(quad->arg1), make_operand(quad->arg2), quad->line);
}

Instruction*
IopCodeDispatcher::generate_DIV(const Quad* quad) {
    assert(quad->op == DIV_t);
    return new Instruction(DIV_VM, make_operand(quad->result), make_operand(quad->arg1), make_operand(quad->arg2), quad->line);
}

Instruction*
IopCodeDispatcher::generate_MOD(const Quad* quad) {
    assert(quad->op == MOD_t);
    return new Instruction(MOD_VM, make_operand(quad->result), make_operand(quad->arg1), make_operand(quad->arg2), quad->line);
}

Instruction*
IopCodeDispatcher::generate_UMINUS(const Quad* quad) {
    assert(quad->op == UMINUS_t);
    return new Instruction(MUL_VM, make_operand(quad->result), make_operand(quad->arg1), make_operand(new IntConstant(-1)), quad->line);
}

Instruction*
IopCodeDispatcher::generate_JUMP(const Quad* quad) {
    assert(quad->op == JUMP_t);
    return new Instruction(JUMP_VM, make_operand(quad->result), nullptr, nullptr, quad->line);
}

Instruction*
IopCodeDispatcher::generate_IF_EQ(const Quad* quad) {
    assert(quad->op == IF_EQ_t);
    return new Instruction(JEQ_VM, make_operand(quad->result), make_operand(quad->arg1), make_operand(quad->arg2), quad->line);
}

Instruction*
IopCodeDispatcher::generate_IF_NOTEQ(const Quad* quad) {
    assert(quad->op == IF_NOTEQ_t);
    return new Instruction(JNE_VM, make_operand(quad->result), make_operand(quad->arg1), make_operand(quad->arg2), quad->line);
}

Instruction*    
IopCodeDispatcher::generate_IF_LESSEQ(const Quad* quad) {
    assert(quad->op == IF_LESSEQ_t);
    return new Instruction(JLE_VM, make_operand(quad->result), make_operand(quad->arg1), make_operand(quad->arg2), quad->line);
}

Instruction*
IopCodeDispatcher::generate_IF_GREATEREQ(const Quad* quad) {
    assert(quad->op == IF_GREATEREQ_t);
    return new Instruction(JGE_VM, make_operand(quad->result), make_operand(quad->arg1), make_operand(quad->arg2), quad->line);
}

Instruction*
IopCodeDispatcher::generate_IF_LESS(const Quad* quad) {
    assert(quad->op == IF_LESS_t);
    return new Instruction(JLT_VM, make_operand(quad->result), make_operand(quad->arg1), make_operand(quad->arg2), quad->line);
}

Instruction*    
IopCodeDispatcher::generate_IF_GREATER(const Quad* quad) {
    assert(quad->op == IF_GREATER_t);
    return new Instruction(JGT_VM, make_operand(quad->result), make_operand(quad->arg1), make_operand(quad->arg2), quad->line);
}

Instruction*    
IopCodeDispatcher::generate_CALL(const Quad* quad) {
    assert(quad->op == CALL_t);
    return new Instruction(CALLFUNC_VM, make_operand(quad->result), nullptr, nullptr, quad->line);
}

Instruction*
IopCodeDispatcher::generate_PARAM(const Quad* quad) {
    assert(quad->op == PARAM_t);
    return new Instruction(PUSHARG_VM, make_operand(quad->result), nullptr, nullptr, quad->line);
}

Instruction*    
IopCodeDispatcher::generate_RETURN(const Quad* quad) {
    assert(quad->op == RET_t);
    return nullptr;
}

Instruction*
IopCodeDispatcher::generate_GETRETVAL(const Quad* quad) {
    assert(quad->op == GETRETVAL_t);
    return nullptr;
}

Instruction*
IopCodeDispatcher::generate_FUNCSTART(const Quad* quad) {
    assert(quad->op == FUNCSTART_t);
    return new Instruction(ENTERFUNC_VM, make_operand(quad->result), nullptr, nullptr, quad->line);
}

Instruction*    
IopCodeDispatcher::generate_FUNCEND(const Quad* quad) {
    assert(quad->op == FUNCEND_t);
    return new Instruction(EXITFUNC_VM, make_operand(quad->result), nullptr, nullptr, quad->line);
}

Instruction*    
IopCodeDispatcher::generate_TABLECREATE(const Quad* quad) {
    assert(quad->op == TABLECREATE_t);
    return new Instruction(NEWTABLE_VM, make_operand(quad->result), nullptr, nullptr, quad->line);
}

Instruction*    
IopCodeDispatcher::generate_TABLEGETELEM(const Quad* quad) {
    assert(quad->op == TABLEGETELEM_t);
    return new Instruction(TABLEGETELEM_VM, make_operand(quad->result), nullptr, nullptr, quad->line);
}

Instruction*    
IopCodeDispatcher::generate_TABLESETELEM(const Quad* quad) {
    assert(quad->op == TABLESETELEM_t);
    return new Instruction(TABLESETELEM_VM, make_operand(quad->result), nullptr, nullptr, quad->line);
}