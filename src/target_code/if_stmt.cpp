#include "../../include/target_code/if_stmt.h"

namespace target_code {
    Quad*
    IfStmt:: get_if_jump_quad() const {
        return if_jump_quad;
    }

    void
    IfStmt:: set_if_jump_quad(Quad* jump_quad) {
        PRECONDITION(jump_quad != nullptr);
        PRECONDITION(IsJumpQuad(jump_quad));
        if_jump_quad = jump_quad;
    }

    void 
    IfStmt:: PatchIfJumpQuad(const unsigned int patch_label) {
        PRECONDITION(patch_label >= 0);
        PatchJumpQuad(if_jump_quad, patch_label);
    }

    void
    IfStmt:: PushElseJumpQuad(Quad* else_jump_quad) {
        PRECONDITION(else_jump_quad != nullptr);
        PRECONDITION(IsJumpQuad(else_jump_quad));
        else_jump_quads.push_back(else_jump_quad);
    }

    void
    IfStmt:: PatchElseJumpQuad(const unsigned int patch_label) {
        PRECONDITION(patch_label >= 0);
        auto back_else_jump_quad = else_jump_quads.back(); 
        PatchJumpQuad(back_else_jump_quad, patch_label);
    }

    void 
    IfStmt:: PopElseJumpQuad() {
        else_jump_quads.pop_back();
    }
}