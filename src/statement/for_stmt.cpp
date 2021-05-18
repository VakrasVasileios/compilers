#include "../../include/statement/for_stmt.h"

void
ForStmt:: PatchLoopQuads() {
    auto expr_jump_quad = loop_quads.back();
    loop_quads.pop_back();

    auto loop_quad = loop_quads.back();
    loop_quads.pop_back();

    auto exit_quad = loop_quads.back();
    loop_quads.pop_back();

    auto branch_quad = loop_quads.back();
    loop_quads.pop_back(); 

    PatchJumpQuad(expr_jump_quad, exprs_first_quad_label);
    PatchJumpQuad(loop_quad, logical_expr_first_quad_label);
    PatchJumpQuad(exit_quad, expr_jump_quad->label+1);
    PatchBranchQuad(branch_quad, loop_quad->label + 1);
}

void
ForStmt:: PatchContinueJumpQuads() {
    for (auto continue_jump_quad : continue_jump_quads)
        PatchJumpQuad(continue_jump_quad, exprs_first_quad_label);
}

unsigned int
ForStmt:: get_logical_expr_first_quad_label() const {
    return logical_expr_first_quad_label;
}

unsigned int
ForStmt:: get_exprs_first_quad_label() const {
    return exprs_first_quad_label;
}

void 
ForStmt:: set_logical_expr_first_quad_label(const unsigned int label) {
    logical_expr_first_quad_label = label;
}

void
ForStmt:: set_exprs_first_quad_label(const unsigned int label) {
    exprs_first_quad_label = label;
}