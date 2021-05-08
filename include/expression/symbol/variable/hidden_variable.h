#ifndef HIDDENVARIABLE_H
#define HIDDENVARIABLE_H

#include "variable.h"

#define HIDDEN_VARIABLE_LINE 0

class HiddenVariable final : public Variable {
public:
    HiddenVariable(std::string id, unsigned int scope) : Variable(HIDDEN, id, HIDDEN_VARIABLE_LINE, scope) {};
    ~HiddenVariable() = default;
protected:
    std::ostream&   LogSymbol(std::ostream& os) const override  { return os; };    
};

#endif