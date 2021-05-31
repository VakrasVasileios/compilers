#ifndef FORSTATEMENT_H
#define FORSTATEMENT_H

#include "loop_stmt.h"

namespace intermediate_code {
    /**
     * @brief A for statement.
    **/ 
    class ForStmt final : public LoopStmt {
    public:
        ForStmt(unsigned int first_quad_label) : LoopStmt(FOR_t, first_quad_label) {};
        ~ForStmt() = default;

        void            PatchLoopQuads() override;
        void            PatchContinueJumpQuads() override;
        /**
         * @brief Returns this ForStmt label of the first quad of the 
         * logical expression with which this ForStmt is evaluated at
         * each loop.
         * e.g the "i<2" expression in the "for (i; i < 2; i++)" stmt
         * has a first quad with a label. Its value is stored in
         * this label.
         * 
         * @return this ForStmt label of the first quad of the 
         * logical expression with which this ForStmt is evaluated at
         * each loop, greater or equal to zero.
        **/ 
        unsigned int    get_logical_expr_first_quad_label() const;
        /**
         * @brief Returns this ForStmt label of the first quad of the 
         * expressions that take execute after this ForStmt main body.
         * e.g the "i++" expression in the "for (i; i < 2; i++)" stmt
         * has an first quad with a label. Its value is stored in this label.
         * 
         * @return this ForStmt label of the first quad of the 
         * expressions that take execute after this ForStmt main body,
         * greater or equal to zero.
        **/ 
        unsigned int    get_exprs_first_quad_label() const;
        /**
         * @brief Sets this ForStmt label of the first quad of the 
         * logical expression with which this ForStmt is evaluated at
         * each loop.
         * e.g the "i<2" expression in the "for (i; i < 2; i++)" stmt
         * has a first quad with a label. Its value is stored in
         * this label.
         * 
         * @param label the label to pass to this ForStmt label of the
         * first quad of the logical expression with which this ForStmt
         * is evaluated at each loop, greater or equal to zero.
        **/ 
        void            set_logical_expr_first_quad_label(const unsigned int label);
        /**
         * @brief Sets this ForStmt label of the first quad of the 
         * expressions that take execute after this ForStmt main body.
         * e.g the "i++" expression in the "for (i; i < 2; i++)" stmt
         * has an first quad with a label. Its value is stored in this label.
         * 
         * @param label the label to pass to this ForStmt label of the
         * first quad oof the expressions that take execute after this
         * ForStmt main body, greater or equal to zero.
        **/ 
        void            set_exprs_first_quad_label(const unsigned int label);

    private:
        unsigned int logical_expr_first_quad_label;
        unsigned int exprs_first_quad_label;  
    };
}

#endif