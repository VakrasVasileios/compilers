#ifndef USERFUNCTION_H
#define USERFUNCTION_H

#include "function.h"

/*
Provides a user function.
*/
class UserFunction final : public Function {
public:
    UserFunction(std::string id, unsigned int line, unsigned int scope, ScopeSpace space, unsigned int offset)
        : Function(USER_FUNC, id, line, scope, space, offset){};

    UserFunction(std::string id, unsigned int line, unsigned int scope, ScopeSpace space, unsigned int offset, std::list<FormalVariable*> formalArguments)
        : Function(USER_FUNC, id, line, scope, space, offset, formalArguments){}; 

    ~UserFunction() = default;    

    std::ostream&                       LogSymbol(std::ostream& os) const override;
};

#endif