#ifndef SYMBOL_H
#define SYMBOL_H

#include "expression.h"
#include <string>
#include <iostream>


/**
 * @brief The scope space of a symbol.
 * Indicates wether a symbol is declared inside/outside of a function,
 * or as a function argument.
**/ 
enum ScopeSpace {
    PROGRAM_VAR,    // out of functions
    FUNCTION_LOCAL, // inside of function
    FORMAL_ARG
};

/**
 * @brief A symbol expression with an id.
 * 
**/ 
class Symbol : public Expression {
public:
    /**
     * @brief Returns a read access to this Symbol id.
     * 
     * @return a read access to this Symbol id, not null
    **/ 
    std::string             get_id() const;
    /**
     * @brief Returns a read access to this Symbol line.
     * 
     * @return a read access to this Symbol line, greater or equal to zero.
     * 
    **/ 
    unsigned int            get_line() const;
    /**
     * @brief Returns a read access to this Symbol scope.
     * 
     * @return a read access to this Symbol scope, greater or equal to zero.
     *
    **/  
    unsigned int            get_scope() const;
    /**
     * @brief Returns a read access to this Symbol activity.
     * Determines wether this Symbol is visible or not at this occurance. 
     * 
     * @return a read access to this Symbol activity
    **/ 
    bool                    is_active() const;
    /**
     * @brief Returns a read access to this Symbol scope space.
     * The scope space of a symbol indicates wether it's declared
     * inside/outside of a function, or as a formal argument.
     * 
     * @return a read access to this Symbol scope space
    **/ 
    ScopeSpace              get_space() const;
    /**
     * @brief Returns a read access to this this Symbol offset.
     * The offset of a symbol indicates its rank on its specific scope space.
     * 
     * @return a read access to this this Symbol offset, greater or equal to zero.
     * 
    **/ 
    unsigned int            get_offset() const;
    /**
     * @brief Sets this Symbol activity.
     * A symbol's activity determines wether the Symbol
     * is visible or not at this occurance. 
     * 
     * @param active the activity to set this Symbol.
    **/ 
    void                    set_active(bool active);

    friend std::ostream&    operator<<(std::ostream& os, const Symbol* symbol);
protected:
    Symbol(ExprType type, std::string id, unsigned int line, unsigned int scope , ScopeSpace space, unsigned int offset) :
    Expression(type), id(id), line(line), scope(scope), active(true), space(space), offset(offset) {};
    
    ~Symbol() = default;
    /**
     * @brief Returns a read access to this Symbol space as a string.
    **/ 
    std::string             space_to_string() const;
     /**
     * @brief Logs this Symbol.
     * 
     * @param os the ostream to log this Symbol
    **/ 
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