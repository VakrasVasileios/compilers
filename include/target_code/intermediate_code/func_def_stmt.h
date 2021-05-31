#ifndef FUNCTIONDEF_H
#define FUNCTIONDEF_H

#include "expression/symbol.h"
#include <list>
#include "quad.h"
#include "stmt.h"

namespace intermediate_code {
    /**
     * @brief A function definition statement.
     * 
    **/ 
    class FuncDefStmt final : public Stmt {
    public:
        FuncDefStmt(expression::Symbol* sym) : Stmt(FUNC_t), sym(sym)  {};
        ~FuncDefStmt() = default;
        /**
         * @brief Returns a read access to this FuncDefStmt symbol that is defined.
         * 
         * @return a read access to this FuncDefStmt symbol that is defined, not null
        **/ 
        expression::Symbol*         get_sym() const;
        /**
         * @brief Returns a read access to this FuncDefStmt current
         * offset, which the symbols defined inside this FuncDefStmt will hold.
         * 
         * @return a read access to this FuncDefStmt current
         * offset, which the symbols defined inside this FuncDefStmt will hold,
         *  greater or equal to zero.
        **/ 
        unsigned int    get_offset() const;
        /**
         * @brief Increments this FuncDefStmt current offset, which the 
         * symbols defined inside this FuncDefStmt will hold.
        **/ 
        void            IncreaseOffset();
        /**
         * @brief Returns a read access to this FuncDefStmt jump quad, that 
         * precedes this FuncDefStmt func start quad.
         * 
         * @return a read access to this FuncDefStmt jump quad, that 
         * precedes this FuncDefStmt func start quad. not null and
         * with a Jump op code.
        **/ 
        Quad*           get_func_start_jump_quad() const;
        /**
         * @brief Sets this FuncDefStmt jump quad, that 
         * precedes this FuncDefStmt func start quad.
         * 
         * @param jump_quad the jump quad that 
         * precedes this FuncDefStmt func start quad, not null
         * and with a Jump op code.
        **/ 
        void            set_func_start_jump_quad(Quad* jump_quad);
        /**
         * @brief Patches this FuncDefStmt jump quad, that 
         * precedes this FuncDefStmt func start quad, label.
         * 
         * @param patch_label the label to patch this FuncDefStmt jump quad, that 
         * precedes this FuncDefStmt func start quad, not null and greater or equal to zero.
        **/ 
        void            PatchFuncStartJumpQuad(const unsigned int patch_label);
        /**
         * @brief Pushes a jump quad from a return statement to
         * this FuncDefStmt list of return jump quads.
         * 
         * @param return_jump_quad the return jump quad to be pushed
         * to this FuncDefStmt list of return jump quads, not null and with
         * Jump op code.
         * 
        **/ 
        void            PushReturnJumpQuad(Quad* return_jump_quad);
        /**
         * @brief Patches this FuncDefStmt list of of return jump quads.
         * 
         * @param patch_label the label to patch this FuncDefStmt list
         * of return jump quads, greater or equal to zero.
         * 
        **/ 
        void            PatchReturnJumpQuads(const unsigned int patch_label);
    private:
        expression::Symbol*             sym;
        unsigned int        offset;
        Quad*               func_start_jump_quad;
        std::list<Quad*>    return_jump_quads;
    };
}

#endif