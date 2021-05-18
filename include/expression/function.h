#ifndef FUNCTION_H
#define FUNCTION_H

#include "symbol.h"
#include "formal_variable.h"
#include <list>

/**
 * @brief A function symbol expression.
**/ 
class Function : public Symbol {
public:
    /**
     * @brief Returns a read access to this Function formal arguments.
     * 
     * @return a read access to this Function formal arguments, not null
    **/ 
    std::list<FormalVariable*>  get_formal_arguments() const; 
protected:
    Function(ExprType type, std::string id, unsigned int line, unsigned int scope, ScopeSpace space, unsigned int offset) : 
    Symbol(type, id, line, scope, space, offset) {};

    Function(ExprType type, std::string id, unsigned int line, unsigned int scope,  ScopeSpace space, unsigned int offset, std::list<FormalVariable*> formalArguments) : 
    Symbol(type, id, line, scope, space, offset), formal_arguments(formalArguments) {};
    
    ~Function() = default; 
private:
    const std::list<FormalVariable*>    formal_arguments;
};

#endif
