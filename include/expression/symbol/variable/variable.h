#ifndef VARIABLE_H
#define VARIABLE_H

#include "../symbol.h"

enum ScopeSpace {
    LOCALVAR,
    GLOBALVAR,
    FORMALVAR,
    HIDDEN
};

class Variable : public Symbol {   
public:
    ScopeSpace       get_space() const;
protected:
    Variable(ScopeSpace scope_type, std::string id, unsigned int line, unsigned int scope)
    : Symbol(VAR, id, line, scope), space(scope_type) {};  

    ~Variable() = default;

    std::ostream&   LogSymbol(std::ostream& os) const override;

    std::string     SpaceToString() const;

private:
    ScopeSpace space;
};

#endif