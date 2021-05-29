#include "../include/instruction_opcodes.h"
#include "string"
#include "assert.h"

std::string IopcodeToString(Iopcode _op) {
    switch(_op) {
        case ASSIGN_t:
            return "assign";
        case ADD_t:
            return "add";
        case SUB_t:
            return "sub";
        case MUL_t:
            return "mul";
        case DIV_t:
            return "div";
        case MOD_t:
            return "mod";
        case UMINUS_t:
            return "uminus";
        case JUMP_t:
            return "jump";    
        case IF_EQ_t:
            return "if_eq";
        case IF_NOTEQ_t:
            return "if_noteq";
        case IF_LESSEQ_t:
            return "if_lesseq";
        case IF_GREATEREQ_t:
            return "if_greatereq";
        case IF_LESS_t:
            return "if_less";
        case IF_GREATER_t:
            return "if_greater";
        case CALL_t:
            return "call";           
        case PARAM_t:
            return "param";          
        case RET_t:
            return "return";  
        case GETRETVAL_t:
            return "getretval";
        case FUNCSTART_t:
            return "funcstart";     
        case FUNCEND_t:
            return "funcend";
        case TABLECREATE_t:
            return "tablecreate";    
        case TABLEGETELEM_t:
            return "tablegetelem";
        case TABLESETELEM_t:
            return "tablesetelem";
        default:
            assert(false);
    }
}