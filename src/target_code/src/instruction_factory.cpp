#include "instruction_factory.h"

namespace target_code 
{
    namespace instruction_factory 
    {
    Instruction* create_instruction(Vmopcode op, const unsigned int label, Vmarg* result,
        const Vmarg* arg1, const Vmarg* arg2, const unsigned int src_line) {
        switch (op) {
            case ASSIGN_VM:
                return new Assign(label, result, arg1, src_line);
            case ADD_VM:
                return new Add(label, result, arg1, arg2, src_line);
            case SUB_VM:
                return new Sub(label, result, arg1, arg2, src_line);
            case MUL_VM:
                return new Mul(label, result, arg1, arg2, src_line);
            case DIV_VM:
                return new Div(label, result, arg1, arg2, src_line);
            case MOD_VM:
                return new Mod(label, result, arg1, arg2, src_line);
            case JEQ_VM:
                return new Jeq(label, result, arg1, arg2, src_line);
            case JNE_VM:
                return new Jne(label, result, arg1, arg2, src_line);
            case JGT_VM:
                return new Jgt(label, result, arg1, arg2, src_line);
            case JLT_VM:
                return new Jlt(label, result, arg1, arg2, src_line);
            case JLE_VM:
                return new Jle(label, result, arg1, arg2, src_line);
            case JGE_VM:
                return new Jge(label, result, arg1, arg2, src_line);                        
            case JUMP_VM:
                return new Jump(label, result, src_line);
            case CALLFUNC_VM:
                return new CallFunc(label, result, src_line);
            case PUSHARG_VM:
                return new PushArg(label, result, src_line);
            case ENTERFUNC_VM:
                return new EnterFunc(label, result, src_line);
            case EXITFUNC_VM:
                return new ExitFunc(label, result, src_line);
            case NEWTABLE_VM:
                return new NewTable(label, result, src_line);
            case TABLEGETELEM_VM:
                return new TableGetElem(label, result, arg1, arg2, src_line);
            case TABLESETELEM_VM:
                return new TableSetElem(label, result, arg1, arg2, src_line);
            default:
                PRECONDITION(false);                                
        }
    }
    }
} 