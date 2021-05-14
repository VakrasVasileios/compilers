#ifndef SYMBOL_H
#define SYMBOL_H

#include "../expression.h"
#include <string>
#include <iostream>

/*
Provides the scope space of a symbol, indicating wether 
it's declared inside/outside of a function, or as a function argument.
*/
enum ScopeSpace {
    PROGRAM_VAR,    // out of functions
    FUNCTION_LOCAL, // inside of function
    FORMAL_ARG
};


/*
Provides a symbol, an expression with an id. 
*/
class Symbol : public Expression {
public:
    /*
    Returns a read access to this Symbol id.
    */
    std::string             get_id() const;
    /*
    Returns a read access to this Symbol line.
    */
    unsigned int            get_line() const;
    /*
    Returns a read access to this Symbol scope.
    */
    unsigned int            get_scope() const;
    /*
    Returns a read access to this Symbol activity.
    Determines wether this Symbol is visible or not at this occurance. 
    */
    bool                    is_active() const;
    /*
    Returns a read access to this Symbol scope space.
    The scope space of a symbol indicates wether it's declared
    inside/outside of a function, or as a formal argument.
    */
    ScopeSpace              get_space() const;
    /*
    Returns a read access to this Symbol offset.
    The offset of a symbol indicates its rank on its specific scope space.
    */
    unsigned int            get_offset() const;
    /*
    Sets this Symbol activity.
    Determines wether this Symbol is visible or not at this occurance. 
    */
    void                    set_active(bool active);

    friend std::ostream&    operator<<(std::ostream& os, const Symbol* symbol);
protected:
    Symbol(ExprType type, std::string id, unsigned int line, unsigned int scope , ScopeSpace space, unsigned int offset) :
    Expression(type), id(id), line(line), scope(scope), active(true), space(space), offset(offset) {};
    
    ~Symbol() = default;
    /*
    Returns a read access to this Symbol space as a string.
    */
    std::string             SpaceToString() const;
    /*
    Logs this Symbol.
    */  
    virtual std::ostream&   LogSymbol(std::ostream& os) const = 0;
    std::ostream&           LogExpression(std::ostream& os) const override; 
private:
    const std::string   id;
    const unsigned int  line;
    const unsigned int  scope;
    bool                active;
    const ScopeSpace    space;
    const unsigned int  offset;
};

#endif