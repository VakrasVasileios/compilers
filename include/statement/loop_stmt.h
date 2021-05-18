#ifndef LOOPSTATEMENT_H
#define LOOPSTATEMENT_H

#include "statement.h"
#include <list>
#include "../quad.h"

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
    /**
     * @brief Pushes a standart loop jump/branch quad to this
     * LoopStmt list of standart loop jump/branch quads.
     * 
     * @param loop_quad the loop_quad to be pushed to this
     * LoopStmt list of standart loop jump/branch quads, not null
     * and with opcode Jump or Branch.
    **/ 
    void            PushLoopQuad(Quad* loop_quad);
    /**
     * @brief Pushes a jump quad from a break statement to this
     * LoopStmt list of jump quad from break statements.
     * 
     * @param break_jump_quad the jump quad from a break statement
     * to be pushed to this LoopStmt list of jump quad from break statements,
     * not null and with Jump opcode.
    **/ 
    void            PushBreakJumpQuad(Quad* break_jump_quad);  
    /**
     * @brief Pushes a jump quad from a continue statement to this
     * LoopStmt list of jump quads from continue statements.
     * 
     * @param continue_jump_quad the jump quad from a continue statement
     * to be pushed to this LoopStmt list of jump quads from continue statements,
     * not null and with Jump opcode.
    **/ 
    void            PushContinueJumpQuad(Quad* continue_jump_quad); 
    /**
     * @brief Patches this LoopStmt list of standart loop branch/jump quads.
    */ 
    virtual void    PatchLoopQuads() = 0;
    /**
     * @brief Patches this LoopStmt list of jump quads from break statements.
     * 
     * @param patch_label the label to patch this LoopStmt list of jump quads from break
     * statements, greater or equal to zero.
    **/ 
    void            PatchBreakJumpQuads(unsigned int patch_label);
    /**
     * @brief Patches this LoopStmt list of jump quads from continue statements.
    **/ 
    virtual void    PatchContinueJumpQuads() = 0;
protected:
    LoopStmt(LoopType loop_type, unsigned int first_quad_label)
        : Statement(Loop_t), loop_type(loop_type), first_quad_label(first_quad_label) {};
    ~LoopStmt() = default;

    std::list<Quad*>    loop_quads;
    std::list<Quad*>    break_jump_quads;
    std::list<Quad*>    continue_jump_quads;
private:
    const LoopType      loop_type;
    const unsigned int  first_quad_label;  
};

#endif