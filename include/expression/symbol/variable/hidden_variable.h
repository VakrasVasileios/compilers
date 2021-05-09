#ifndef HIDDENVARIABLE_H
#define HIDDENVARIABLE_H

#include "variable.h"

#define HIDDEN_VARIABLE_LINE 0

/*
Provides a hidden variable, used at the production of the intermediate code.
This variable is stored at the symbol table, but it's registered
as an excluded from the source code entity.
*/
class HiddenVariable final : public Variable {
public:
    HiddenVariable(std::string id, unsigned int scope) : Variable(HIDDEN, id, HIDDEN_VARIABLE_LINE, scope) {};
    ~HiddenVariable() = default;
};

#endif