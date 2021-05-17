#ifndef QUAD_H
#define QUAD_H

#include "expression/expression.h"
#include "expression/int_constant.h"
#include "instruction_opcodes.h"
#include <iostream>


/**
 * @brief A quad, the building stone of the intermediate code.
**/
typedef struct quad {
    Iopcode       op;
    Expression*   result;
    Expression*   arg1;
    Expression*   arg2;
    unsigned int  label;
    unsigned int  line;

    quad(Iopcode _op, Expression* _res, Expression* _arg1, Expression* _arg2, unsigned int _label, unsigned int _line)
    : op(_op), result(_res), arg1(_arg1), arg2(_arg2), label(_label), line(_line) {};
    /**
     * @brief Patches this jump Quad label.
     * It's a checked runtime error for this Quad to
     * not have a Jump op code.
     * 
     * @param label the label to patch this jump Quad
     * label, greater or equal to zero
    **/ 
    void                    PatchJumpQuad(const unsigned int label);
    /**
     * @brief Patches this branch Quad arg2.
     * It's a checked runtime error for this Quad to
     * not have a Branch op code.
     * 
     * @param label the label to patch this branch Quad
     * arg2, greater or equal to zero
    **/ 
    void                    PatchBranchQuad(const unsigned int label);

    friend std::ostream&    operator<<(std::ostream& os, const quad _quad);
} Quad;

#endif