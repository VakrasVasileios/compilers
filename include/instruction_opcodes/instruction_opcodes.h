#ifndef INSTRUCTIONOPCODES_H
#define INSTRUCTIONOPCODES_H

#include <string>

/*
Provides all of the instruction op codes.
*/
enum Iopcode {
    ASSIGN_t,   //Assign Operation.
    ADD_t,            SUB_t,            MUL_t,        DIV_t,            MOD_t,  //Arithmetic Operation.
    UMINUS_t,         AND_t,            OR_t,         NOT_t,        //Arithmetic Operation.
    JUMP_t,
    IF_EQ_t,          IF_NOTEQ_t,       IF_LESSEQ_t,  IF_GREATEREQ_t,   IF_LESS_t,    IF_GREATER_t,     //Jump Operation.
    CALL_t,           PARAM_t,          RET_t,        GETRETVAL_t,      //Function Operation.
    FUNCSTART_t,      FUNCEND_t,
    TABLECREATE_t,    TABLEGETELEM_t,   TABLESETELEM_t  //Special Operation.
};

/*
Returns a read/write access to an opcode as a string.
*/
std::string OpCodeToString(Iopcode _op);

#endif