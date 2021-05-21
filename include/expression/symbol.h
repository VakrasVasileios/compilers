#ifndef SYMBOL_H
#define SYMBOL_H

#include "primary.h"
#include <string>
#include <iostream>
#include <list>
#include "../../util/contract/contract.h"


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
 * @brief An lvalue primary expression.
 * 
**/ 
class Symbol : public Primary {
public:
    /**
     * @brief Constructs a new Symbol object.
     * 
     * @param type the expression type of this Symbol, must be of LIB_FUNC,
     * USER_FUNC, VAR type, or TABLE_ITEM
     * @param id the unique identifier of this Symbol, not empty
     * @param line the line of this Symbol, greater or equal to zero.
     * @param scope the scope of this Symbol, greater or equal to zero.
     * @param space the scope space of this Symbol, indicating wether this
     * Symbol has been declared inside/outside of a function definition, or
     * as a formal argument.
     * @param offset the offset of this Symbol at its scope space, greater
     * or equal to zero.
     */
    Symbol(ExprType type, std::string id, unsigned int line, unsigned int scope , ScopeSpace space, unsigned int offset) :
        Primary(verify_type(type)), id(verify_id(id)), line(verify_line(line)), scope(verify_scope(scope)),
        active(true), space(space), offset(verify_offset(offset)) {};
    /**
     * @brief Destroys this Symbol object.
     * 
     */
    ~Symbol() = default;
    /**
     * @brief Returns a read access to this Symbol id.
     * 
     * @return a read access to this Symbol id, not null
    **/ 
    std::string                 get_id() const;
    /**
     * @brief Returns a read access to this Symbol line.
     * 
     * @return a read access to this Symbol line, greater or equal to zero.
     * 
    **/ 
    unsigned int                get_line() const;
    /**
     * @brief Returns a read access to this Symbol scope.
     * 
     * @return a read access to this Symbol scope, greater or equal to zero.
     *
    **/  
    unsigned int                get_scope() const;
    /**
     * @brief Returns a read access to this Symbol activity.
     * Determines wether this Symbol is visible or not at this occurance. 
     * 
     * @return a read access to this Symbol activity
    **/ 
    bool                        is_active() const;
    /**
     * @brief Returns a read access to this Symbol scope space.
     * The scope space of a symbol indicates wether it's declared
     * inside/outside of a function, or as a formal argument.
     * 
     * @return a read access to this Symbol scope space
    **/ 
    ScopeSpace                  get_space() const;
    /**
     * @brief Returns a read access to this this Symbol offset.
     * The offset of a symbol indicates its rank on its specific scope space.
     * 
     * @return a read access to this this Symbol offset, greater or equal to zero.
     * 
    **/ 
    unsigned int                get_offset() const;
    /**
     * @brief Returns a read access to this Symbol formal arguments.
     * 
     * @return a read access to this Symbol formal arguments, 
     * all have a FORMAL_ARG scope space.
     */
    std::list<Symbol*>          get_formal_arguments() const;
    /**
     * @brief Sets this Symbol activity.
     * A symbol's activity determines wether the Symbol
     * is visible or not at this occurance. 
     * 
     * @param active the activity to set this Symbol.
    **/ 
    void                        set_active(bool active);
    /**
     * @brief Checks wether this Symbol has already
     * included a formal argument to its list of
     * formal arguments.
     * 
     * @param formal_arg the formal argument to be checked, not null
     * and with a FORMAL_ARG scope space.
     *  
     * @return wether this Symbol has already
     * included a formal argument to its list of
     * formal arguments
     */
    bool                        HasFormalArg(Symbol* formal_arg); 
    /**
     * @brief Adds a formal argument to this Symbol list of
     * formal arguments. 
     * 
     * @param formal_arg the formal argument to be checked, not null,
     * with a FORMAL_ARG scope space and not already included to this
     * Symbol list of formal arguments.
     */
    void                        AddFormalArg(Symbol* formal_arg);                   

    friend std::ostream&        operator<<(std::ostream& os, const Symbol* symbol);    
    std::string                 to_string() const override;
private:
    const std::string           id;
    const unsigned int          line;
    const unsigned int          scope;
    bool                        active;
    const ScopeSpace            space;
    const unsigned int          offset;
    std::list<Symbol*>          formal_arguments;

    ExprType                    verify_type(ExprType type);
    std::string                 verify_id(std::string id);
    unsigned int                verify_line(unsigned int line);
    unsigned int                verify_scope(unsigned int scope);
    unsigned int                verify_offset(unsigned int offset);
    bool                        is_state_valid();
    std::string                 space_to_string() const;
    std::string                 sym_to_string() const;
    std::ostream&               LogExpression(std::ostream& os) const override;
};

#endif