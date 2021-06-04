#include "vm_opcodes.h"
#include <assert.h>

namespace target_code {
    std::string
    VmopcodeToString(Vmopcode _op) {
        switch(_op) {
            case ASSIGN_VM:
                return "assign";
            case ADD_VM:
                return "add";
            case SUB_VM:
                return "sub";
            case MUL_VM:
                return "mul";
            case DIV_VM:
                return "div";
            case MOD_VM:
                return "mod";
            case JUMP_VM:
                return "jump";    
            case JEQ_VM:
                return "jeq";
            case JNE_VM:
                return "jne";
            case JLE_VM:
                return "jle";
            case JGE_VM:
                return "jge";
            case JLT_VM:
                return "jlt";
            case JGT_VM:
                return "jgt";
            case CALLFUNC_VM:
                return "callfunc";           
            case PUSHARG_VM:
                return "pusharg";          
            case ENTERFUNC_VM:
                return "enterfunc";     
            case EXITFUNC_VM:
                return "exitfunc";
            case NEWTABLE_VM:
                return "newtable";    
            case TABLEGETELEM_VM:
                return "tablegetelem";
            case TABLESETELEM_VM:
                return "tablesetelem";
            default:
                assert(false);
        }
    }
}