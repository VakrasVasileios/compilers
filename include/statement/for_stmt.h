#ifndef FORSTATEMENT_H
#define FORSTATEMENT_H

#include "loop_stmt.h"

/**
 * @brief A for statement.
**/ 
class ForStmt final : public LoopStmt {
public:
    ForStmt(unsigned int first_quad_label) : LoopStmt(FOR_t, first_quad_label) {};
    ~ForStmt() = default;
};

#endif