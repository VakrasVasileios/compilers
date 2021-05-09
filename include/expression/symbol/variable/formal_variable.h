#ifndef FORMALVARIABLE_H
#define FORMALVARIABLE_H

#include "variable.h"

/*
Provides a formal variable.
Formal variables are variable symbols declared as function parameters.
*/
class FormalVariable final : public Variable {
public:
    FormalVariable(std::string id, unsigned int line, unsigned int scope) : Variable(FORMALVAR, id, line, scope) {};
    ~FormalVariable() = default;
};

#endif