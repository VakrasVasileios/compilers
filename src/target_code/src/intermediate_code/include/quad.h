#ifndef QUAD_H
#define QUAD_H

#include <iostream>
#include "../../../../../util/contract/include/contract.h"
#include "../../syntax_analysis/include/syntax_analysis.h"
#include "instruction_opcodes.h"

namespace target_code
{
    namespace intermediate_code 
    {
    /**
     * @brief A quad, the building stone of the intermediate code.
    **/
    typedef struct quad {
        Iopcode       op;
        expression::Expression*   result;
        expression::Expression*   arg1;
        expression::Expression*   arg2;
        unsigned int  label;
        unsigned int  line;
        unsigned int  taddress;

        quad(Iopcode _op, expression::Expression* _res,
            expression::Expression* _arg1, expression::Expression* _arg2,
                unsigned int _label, unsigned int _line)
        : op(_op), result(_res), arg1(_arg1), arg2(_arg2), label(_label),
            line(_line) {};
    
        friend std::ostream&    operator<<(std::ostream& os, const quad _quad);
    } Quad;

    /**
     * @brief Checks if a quad has a jump opcode.
     * 
     * @param quad the quad to check wether it has a
     * jump opcode, not null
     * 
     * @return wether a quad has a jump opcode.
     * 
    **/ 
    bool    IsJumpQuad(Quad* quad);

    /**
     * @brief Checks if a quad has a branch opcode.
     * 
     * @param quad the quad to check wether it has a
     * branch opcode, not null
     * 
     * @return wether a quad has a branch opcode.
     * 
    **/ 
    bool    IsBranchQuad(Quad* quad);

    /**
     * @brief Patches a jump Quad's label.
     * 
     * @param jump_quad the jump quad to patch, not null
     * and with a Jump opcode.
     * @param label the label to patch the jump Quad
     * label, greater or equal to zero
    **/ 
    void    PatchJumpQuad(Quad* jump_quad, const unsigned int label);

    /**
     * @brief Patches a branch Quad's arg2.
     * 
     * @param branch_quad the branch quad to patch, not null
     * and with a Branch opcode.
     * @param label the label to patch the branch Quad's
     * arg2, greater or equal to zero
    **/ 
    void    PatchBranchQuad(Quad* branch_quad, const unsigned int label);
    }
}

#endif