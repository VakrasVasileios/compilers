#ifndef LOOPSTATEMENT_H
#define LOOPSTATEMENT_H

#include "statement.h"

/**
 * @brief The type of a loop statement.
**/
enum LoopType {
    WHILE_t,
    FOR_t
};

/**
 * @brief An abstract statement of loop type.
**/
class LoopStmt : public Statement {
public:
    /**
     * @brief Returns a read access to the type of this LoopStatement.
     * 
     * @return a read access to the type of this LoopStatement, not null
    **/
    LoopType        get_loop_type() const;
    /**
     * @brief Returns a read access to the label of the first produced
     * quad of this LoopStatement.
     * 
     * @return a read access to the label of the first produced
     * quad of this LoopStatement, not null
    **/
    unsigned int    get_first_quad_label() const;    
protected:
    LoopStmt(LoopType loop_type, unsigned int first_quad_label)
        : Statement(Loop_t), loop_type(loop_type), first_quad_label(first_quad_label) {};
    ~LoopStmt() = default;
private:
    const LoopType      loop_type;
    const unsigned int  first_quad_label;
};

#endif