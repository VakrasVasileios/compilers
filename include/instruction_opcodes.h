#ifndef INSTRUCTIONOPCODES_H
#define INSTRUCTIONOPCODES_H

#include <string>


/**
 * @brief The instruction opcodes.
**/ 
enum Iopcode {
    ASSIGN_t,   
    ADD_t,            SUB_t,            MUL_t,        DIV_t,            MOD_t,  
    UMINUS_t,                
    JUMP_t,
    IF_EQ_t,          IF_NOTEQ_t,       IF_LESSEQ_t,  IF_GREATEREQ_t,   IF_LESS_t,    IF_GREATER_t,   
    CALL_t,           PARAM_t,          RET_t,        GETRETVAL_t,      
    FUNCSTART_t,      FUNCEND_t,
    TABLECREATE_t,    TABLEGETELEM_t,   TABLESETELEM_t 
};

/**
 * @brief Returns a read/write access to an opcode as a string.
 * 
 * @return a read/write access to an opcode as a string, not null
**/ 
std::string IopcodeToString(Iopcode _op);

#endif