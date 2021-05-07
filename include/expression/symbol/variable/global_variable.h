#ifndef GLOBALVARIABLE_H
#define GLOBALVARIABLE_H

#include "variable.h"

class GlobalVariable final : public Variable {
public:
    GlobalVariable(std::string id, unsigned int line, unsigned int scope) : Variable(GLOBALVAR, id, line, scope) {};
    ~GlobalVariable() = default;
};

#endif