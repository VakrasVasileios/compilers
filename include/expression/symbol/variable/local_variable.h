#ifndef LOCALVARIABLE_H
#define LOCALVARIABLE_H

#include "variable.h"

/*
Provides a global variable symbol.
This symbol has a local scope space.
*/
class LocalVariable final : public Variable {
public:
    LocalVariable(std::string id, unsigned int line, unsigned int scope) : Variable(LOCALVAR, id, line, scope) {};
    ~LocalVariable() = default;
};

#endif