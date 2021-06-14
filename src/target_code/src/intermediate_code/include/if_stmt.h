#ifndef IFSTMT_H
#define IFSTMT_H

#include <list>
#include "quad.h"
#include "stmt.h"

namespace target_code
{
    namespace intermediate_code 
    {
    /**
     * @brief An if statement.
    **/ 
    class IfStmt final : public Stmt {
    public:
        IfStmt() : Stmt(IF_t) {}
        ~IfStmt() = default;
        /**
         * @brief Returns this IfStmt jump quad, which jumps out of
         * this IfStmt body in case of a false evaluation.
         * 
         * @return this IfStmt jump quad, which jumps out of
         * this IfStmt body in case of a false evaluation, not null
         * and with a Jump opcode.
        **/ 
        Quad*   get_if_jump_quad() const;
        /**
         * @brief Sets this IfStmt jump quad, which jumps out of
         * this IfStmt body in case of a false evaluation.
         * 
         * @param jump_quad the jump quad to be set as this IfStmt
         * jump quad, which jumps out of this IfStmt body in case
         * of a false evaluation, not null and with a Jump opcode.
        **/  
        void    set_if_jump_quad(Quad* jump_quad);
        /**
         * @brief Patches this IfStmt jump quad, which jumps out of
         * this IfStmt body in case of a false evaluation.
         * 
         * @param patch_label the label to patch this IfStmt jump
         * quad, which jumps out of this IfStmt body in case of a
         * false evaluation, greater or equal to zero
         * 
        **/ 
        void    PatchIfJumpQuad(const unsigned int patch_label);
        /**
         * @brief Pushes a jump quad from an else statement to
         * this IfStmt list of jump quads from else statements.
         * 
         * @param else_jump_quad the jump quad to be pushed to 
         * this IfStmt list of jump quads from else statements,
         * not null and with a Jump opcode.
        **/ 
        void    PushElseJumpQuad(Quad* else_jump_quad);
        /**
         * @brief Patches this IfStmt list of jump quads last element.
         * 
         * @param patch_label the label to patch with this
         * IfStmt list of jump quads last element, greater or equal to zero.
        **/ 
        void    PatchElseJumpQuad(const unsigned int patch_label);
        /**
         * @brief Pops the last element from this IfStmt list of jump quads.
        **/ 
        void    PopElseJumpQuad();
    private:
        Quad*               if_jump_quad;
        std::list<Quad*>    else_jump_quads;
    };
    }
}

#endif