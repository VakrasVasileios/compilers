#ifndef USERFUNCTION_H
#define USERFUNCTION_H

#include "function.h"


class UserFunction final : public Function {
public:
    UserFunction(std::string id, unsigned int line, unsigned int scope) : Function(USER_FUNC, id, line, scope){};

    UserFunction(std::string id, unsigned int line, unsigned int scope, std::list<FormalVariable*> formalArguments)
        : Function(USER_FUNC, id, line, scope, formalArguments){}; 

    ~UserFunction() = default;    
};

#endif