#ifndef GLOBALVARIABLE_H
#define GLOBALVARIABLE_H

#include "variable.h"

/*
Provides a global variable symbol.
This symbol has a global scope space.
*/
class GlobalVariable final : public Variable {
public:
    GlobalVariable(std::string id, unsigned int line, unsigned int scope, unsigned int offset)
        : Variable(id, line, scope, PROGRAM_VAR, offset) {};
    ~GlobalVariable() = default;
protected:
    virtual std::ostream&   LogSymbol(std::ostream& os) const override;    
};

#endif