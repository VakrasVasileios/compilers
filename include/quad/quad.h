#ifndef QUAD_H
#define QUAD_H

#include "../expression/expression.h"
#include "../instruction_opcodes/instruction_opcodes.h"
#include <iostream>


typedef struct quad {
    Iopcode       op;
    Expression*   result;
    Expression*   arg1;
    Expression*   arg2;
    unsigned int        label;
    unsigned int  line;

    quad(Iopcode _op, Expression* _res, Expression* _arg1, Expression* _arg2, unsigned int _label, unsigned int _line)
    : op(_op), result(_res), arg1(_arg1), arg2(_arg2), label(_label), line(_line) {};

    friend std::ostream&    operator<<(std::ostream& os, const quad _quad);
} Quad;

std::string
OpCodeToString(Iopcode _op);

#endif