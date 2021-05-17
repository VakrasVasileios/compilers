#include "../../include/statement/loop_stmt.h"


LoopType
LoopStmt:: get_loop_type() const {
    return loop_type;
}

unsigned int   
LoopStmt:: get_first_quad_label() const {
    return first_quad_label;
}