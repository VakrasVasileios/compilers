#ifndef TABLEMAKEPAIRS_H
#define TABLEMAKEPAIRS_H

#include "tablemake.h"
#include <list>
#include "../../util/contract/contract.h"

/**
 * @brief A table make statement for storing pairs of expressions.
 * 
 */
// class TableMakePairs final : public TableMake {
// public:
//     /**
//      * @brief Constructs a new TableMakePairs object.
//      * 
//      */
//     TableMakePairs() = default;
//     /**
//      * @brief Destroys this TableMakePairs object.
//      * 
//      */
//     ~TableMakePairs() = default;
//     /**
//      * @brief Returns a read access to this TableMakePairs list of 
//      * pairs of expressions.
//      * 
//      * @return a read access to this TableMakePairs list of 
//      * pairs of expressions
//      */
//     std::list<std::pair<Expression*, Expression*>> get_pairs() const;
//     /**
//      * @brief Adds a pair of expressions to this TableMakePairs list of 
//      * pairs of expressions, consisting of an expression index and an expression
//      * value.
//      * 
//      * @param index the index expression to be paired, not null
//      * @param value the value expression to be paired, not null
//      */
//     void                                            AddPair(Expression* index, Expression* value);   
// private:
//     std::list<std::pair<Expression*, Expression*>>  pairs;
// };

#endif