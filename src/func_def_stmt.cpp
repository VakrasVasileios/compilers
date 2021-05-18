#include "../include/func_def_stmt.h"

Symbol*   
FuncDefStmt:: get_sym() const {
    return sym;
}

unsigned int
FuncDefStmt:: get_offset() const {
    return offset;
}

void 
FuncDefStmt:: IncreaseOffset() {
    offset++;
}

Quad*
FuncDefStmt:: get_func_start_jump_quad() const {
    return func_start_jump_quad;
}

void 
FuncDefStmt:: set_func_start_jump_quad(Quad* jump_quad) {
    PRECONDITION(jump_quad != nullptr);
    PRECONDITION(IsJumpQuad(jump_quad));
    func_start_jump_quad = jump_quad;
}

void 
FuncDefStmt:: PatchFuncStartJumpQuad(const unsigned int patch_label) {
    PRECONDITION(patch_label >= 0);
    PatchJumpQuad(func_start_jump_quad, patch_label);
}

void 
FuncDefStmt:: PushReturnJumpQuad(Quad* return_jump_quad) {
    PRECONDITION(return_jump_quad != nullptr);
    PRECONDITION(IsJumpQuad(return_jump_quad));
    return_jump_quads.push_back(return_jump_quad);
}

void
FuncDefStmt:: PatchReturnJumpQuads(const unsigned int patch_label) {
    PRECONDITION(patch_label >= 0);
    for (auto return_jump_quad : return_jump_quads)
        PatchJumpQuad(return_jump_quad, patch_label);
}