#include "../../../../../include/target_code/intermediate_code/syntax_analysis/expression/tablemake_pairs.h"

namespace expression {
    Indexed* 
    TableMakePairs::get_indexed() const {
        return indexed;
    }

    void
    TableMakePairs::Accept(ExpressionVisitor* expr_visitor) {
        PRECONDITION(expr_visitor != nullptr);
        expr_visitor->VisitTableMakePairs(this);
    }

    Indexed*    
    TableMakePairs::verify_indexed(Indexed* indexed) {
        PRECONDITION(indexed != nullptr);
        return indexed;
    }
}