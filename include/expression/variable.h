#ifndef VARIABLE_H
#define VARIABLE_H

#include "symbol.h"

/**
 * @brief A variable symbol expression.
**/ 
class Variable : public Symbol {   
protected:
    Variable(std::string id, unsigned int line, unsigned int scope, ScopeSpace space, unsigned int offset)
    : Symbol(VAR, id, line, scope, space, offset){};  

    ~Variable() = default;
};

#endif