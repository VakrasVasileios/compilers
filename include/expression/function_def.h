#ifndef FUNCTIONDEF_H
#define FUNCTIONDEF_H

#include "expression.h"
#include "symbol/symbol.h"

/*
Provides a user function definition expression.
*/
class FunctionDef final : public Expression {
public:
    FunctionDef(Symbol* sym) :
     Expression(FUNCTION_DEF), sym(sym) {};

    ~FunctionDef() = default;
    /*
    Returns a read access to this FunctionDef defined user function.
    */
    Symbol*         get_sym() const;
    /*
    Returns a read access to this FunctionDef current offset, 
    which the symbols defined inside this FunctionDef will hold.
    */
    unsigned int    get_offset() const;
    /*
    Increases this FunctionDef current offset, 
    which the symbols defined inside this FunctionDef will hold.
    */
    void            IncreaseOffset();
protected:
    std::ostream&   LogExpression(std::ostream& os) const override;
private:
    Symbol*         sym;
    unsigned int    offset;
};

#endif