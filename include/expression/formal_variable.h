#ifndef FORMALVARIABLE_H
#define FORMALVARIABLE_H

#include "variable.h"


/**
 * @brief A formal variable symbol expression.
 * Formal variables are variable symbols declared as function parameters,
 * with a formal scope space.
 * 
**/ 
class FormalVariable final : public Variable {
public:
    FormalVariable(std::string id, unsigned int line, unsigned int scope, unsigned int offset)
        : Variable(id, line, scope, FORMAL_ARG, offset) {};
    ~FormalVariable() = default;
protected:
    virtual std::ostream&   LogSymbol(std::ostream& os) const override;    
};

#endif