#include "../../include/statement/if_stmt.h"

Quad*
IfStmt:: get_if_jump_quad() const {
    return if_jump_quad;
}

void
IfStmt:: set_if_jump_quad(Quad* jump_quad) {
    if_jump_quad = jump_quad;
}

void 
IfStmt:: PatchIfJumpQuad(const unsigned int patch_label) {
    PatchBranchQuad(if_jump_quad, patch_label);
    //erase
}

void
IfStmt:: PushElseJumpQuad(Quad* else_jump_quad) {
    else_jump_quads.push_back(else_jump_quad);
}

void
IfStmt:: PatchElseJumpQuad(const unsigned int patch_label) {
    auto back_else_jump_quad = else_jump_quads.back(); 
    PatchJumpQuad(back_else_jump_quad, patch_label);
}

void 
IfStmt:: PopElseJumpQuad() {
    else_jump_quads.pop_back();
}