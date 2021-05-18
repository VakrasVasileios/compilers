#include "../include/instruction_opcodes.h"
#include "string"
#include "assert.h"

std::string IopcodeToString(Iopcode _op) {
    switch(_op) {
        case ASSIGN_t:
            return "ASSIGN";
        case ADD_t:
            return "ADD";
        case SUB_t:
            return "SUB";
        case MUL_t:
            return "MUL";
        case DIV_t:
            return "DIV";
        case MOD_t:
            return "MOD";
        case UMINUS_t:
            return "UMINUS";
        case AND_t:
            return "AND";
        case OR_t:
            return "OR";
        case NOT_t:
            return "NOT";
        case JUMP_t:
            return "JUMP";    
        case IF_EQ_t:
            return "IF_EQ";
        case IF_NOTEQ_t:
            return "IF_NOTEQ";
        case IF_LESSEQ_t:
            return "IF_LESSEQ";
        case IF_GREATEREQ_t:
            return "IF_GREATEREQ";
        case IF_LESS_t:
            return "IF_LESS";
        case IF_GREATER_t:
            return "IF_GREATER";
        case CALL_t:
            return "CALL";           
        case PARAM_t:
            return "PARAM";          
        case RET_t:
            return "RETURN";  
        case GETRETVAL_t:
            return "GETRETVAL";
        case FUNCSTART_t:
            return "FUNCSTART";     
        case FUNCEND_t:
            return "FUNCEND";
        case TABLECREATE_t:
            return "TABLECREATE";    
        case TABLEGETELEM_t:
            return "TABLEGETELEM";
        case TABLESETELEM_t:
            return "TABLESETELEM";
        default:
            assert(false);
    }
}