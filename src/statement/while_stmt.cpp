#include "../../include/statement/while_stmt.h"

void
WhileStmt:: PatchLoopQuads() {
    auto loop_quad = loop_quads.back();
    loop_quads.pop_back();

    auto exit_quad = loop_quads.back();
    loop_quads.pop_back();

    auto branch_quad = loop_quads.back();
    loop_quads.pop_back();

    PatchJumpQuad(loop_quad, get_first_quad_label());
    PatchJumpQuad(exit_quad, loop_quad->label + 1);
    PatchBranchQuad(branch_quad, branch_quad->label + 2);
}

void
WhileStmt:: PatchContinueJumpQuads() {
    for (auto continue_jump_quad : continue_jump_quads)
        PatchJumpQuad(continue_jump_quad, get_first_quad_label()); 
}