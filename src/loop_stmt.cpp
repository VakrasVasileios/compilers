#include "../include/loop_stmt.h"


LoopType
LoopStmt:: get_loop_type() const {
    return loop_type;
}

unsigned int   
LoopStmt:: get_first_quad_label() const {
    return first_quad_label;
}

void
LoopStmt:: PushLoopQuad(Quad* loop_quad) {
    PRECONDITION(loop_quad != nullptr);
    PRECONDITION(IsJumpQuad(loop_quad) || IsBranchQuad(loop_quad));
    loop_quads.push_back(loop_quad);
}

void 
LoopStmt:: PushBreakJumpQuad(Quad* break_jump_quad) {
    PRECONDITION(break_jump_quad != nullptr);
    PRECONDITION(IsJumpQuad(break_jump_quad));
    break_jump_quads.push_back(break_jump_quad);
}

void 
LoopStmt:: PushContinueJumpQuad(Quad* continue_jump_quad) {
    PRECONDITION(continue_jump_quad != nullptr);
    PRECONDITION(IsJumpQuad(continue_jump_quad));
    continue_jump_quads.push_back(continue_jump_quad);
}

void
LoopStmt:: PatchBreakJumpQuads(unsigned int patch_label) {
    PRECONDITION(patch_label >= 0);
    for (auto break_jump_quad : break_jump_quads)
        PatchJumpQuad(break_jump_quad, patch_label);
}

