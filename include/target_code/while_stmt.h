#ifndef WHILESTATEMENT_H
#define WHILESTATEMENT_H

#include "loop_stmt.h"

namespace target_code {
    /**
     * @brief A while statement.
    **/ 
    class WhileStmt final : public LoopStmt {
    public:
        WhileStmt(unsigned int first_quad_label) : LoopStmt(WHILE_t, first_quad_label) {};
        ~WhileStmt() = default;

        void    PatchLoopQuads() override;
        void    PatchContinueJumpQuads() override;
    };
}

#endif