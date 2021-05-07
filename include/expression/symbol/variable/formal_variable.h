#ifndef FORMALVARIABLE_H
#define FORMALVARIABLE_H

#include "variable.h"

class FormalVariable final : public Variable {
public:
    FormalVariable(std::string id, unsigned int line, unsigned int scope) : Variable(FORMALVAR, id, line, scope) {};
    ~FormalVariable() = default;
};

#endif