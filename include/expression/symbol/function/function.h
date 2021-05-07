#ifndef FUNCTION_H
#define FUNCTION_H

#include "../symbol.h"
#include "../variable/formal_variable.h"
#include <list>

class Function : public Symbol {
public:
    std::list<FormalVariable*>  get_formal_arguments() const; 
protected:
    Function(ExprType type, std::string id, unsigned int line, unsigned int scope) : 
    Symbol(type, id, line, scope) {};

    Function(ExprType type, std::string id, unsigned int line, unsigned int scope, std::list<FormalVariable*> formalArguments) : 
    Symbol(type, id, line, scope), formal_arguments(formalArguments) {};
    
    ~Function() = default;

    std::ostream&                       LogSymbol(std::ostream& os) const override;    
private:
    const std::list<FormalVariable*>    formal_arguments;
};

#endif
