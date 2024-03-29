#ifndef SYNTAX_ANALYSIS_H
#define SYNTAX_ANALYSIS_H

#include <iostream>
#include "../../../../../util/contract/include/contract.h"
#include "../../expression/include/expression.h"

namespace target_code
{
    namespace syntax_analysis 
    {
    /**
     * @brief Checks wether no errors have been signaled during the
     * syntax analysis.
     * 
     * @return wether no errors have been signaled during the
     * syntax analysis
     */
    bool                    NoErrorSignaled();
    /**
     * @brief Signals a syntax error with a message,
     * at a src line.
     * 
     * @param msg the message of the syntax error
     * to be signaled
     * @param line the line where the syntax error is signaled,
     * greater or equal to zero
     */
    void                    SignalError(std::string msg, unsigned int line);
    /**
     * @brief Logs the symbol table to an output.
     * 
     * @param output the output to log the symbol
     * table
     */
    void                    LogSymTable(std::ostream& output); 
    /**
     * @brief Inserts the library functions to the symbol table
     * 
     */
    void                    InitLibraryFunctions();
    /**
     * @brief Returns a read/write access to the current scope.
     * 
     * @return a read/write access to the current scope, greater or
     * equal to zero
     */
    unsigned int            CurrScope();
    /**
     * @brief Increases the current scope.
     * 
     */
    void                    IncreaseScope();
    /**
     * @brief Decreases the current scope.
     * 
     */
    void                    DecreaseScope();
    /**
     * @brief Stores the function local offset.
     * 
     */
    void                    StoreFuncLocalOffset(void);
    /**
     * @brief Restores the function local offset.
     * It's a checked runtime error for no previously
     * stored function local offsets.
     * 
     */
    void                    RestoreFuncLocalOffset(void);
    /**
     * @brief Resets the function local offset.
     * 
     */
    void                    ResetFuncLocalOffset(void);
    /**
     * @brief Resets the formal argument offset.
     * 
     */
    void                    ResetFormalArgOffset(void);
    /**
     * @brief Returns a read/write access to the current
     * scope space.
     * 
     * @return a read/write access to the current
     * scope space 
     */
    expression::ScopeSpace  CurrScopeSpace(void);

    unsigned int
    GetProgramVarOffset(void);
    /**
     * @brief Returns a read/write access to the current
     * scope space offset.
     * 
     * @return a read/write access to the current
     * scope space offset 
     */
    unsigned int            CurrScopeOffset(void);
    /**
     * @brief Increases the current offset.
     * 
     */
    void                    IncreaseCurrOffset(void);
    /**
     * @brief Enters a scope space.
     * 
     */
    void                    EnterScopeSpace(void);
    /**
     * @brief Exits a scope space.
     * It's a checked runtime error for no previously 
     * entered scope spaces.
     */
    void                    ExitScopeSpace(void);
    /**
     * @brief Enables all of the inserted, to the symbol table,
     * symbols across all scopes, except the global scope.
     * 
     */
    void                    EnableLowerScopes();
    /**
     * @brief Hides all of the inserted, to the symbol table,
     * symbols across all scopes, except the global scope.
     * 
     */
    void                    HideLowerScopes();
    /**
     * @brief Searches for a symbol with an id on the symbol table,
     * across all scopes.
     * 
     * @param id the id of the symbol to be searched, not null
     * 
     * @return the symbol with the id on the symbol table, across
     * all scopes if it's found, else nullptr 
     */
    expression::Symbol*     Lookup(const char* id);
    /**
     * @brief Searches for a symbol with an id on the symbol table,
     * across the global scope.
     * 
     * @param id the id of the symbol to be searched, not null
     * 
     * @return the symbol with the id on the symbol table, across
     * the global scope if it's found, else nullptr 
     */
    expression::Symbol*     LookupGlobal(const char* id);
    /**
     * @brief Inserts a new symbol, with a type, an id, an expression index
     * (in case of a table item), parsed at a src line, to the symbol table.
     * 
     * @param type the type of the symbol
     * @param id the id of the symbol, not null 
     * @param index the index of the symbol, can be null 
     * @param line the line on which the symbol is parsed, greater or equal
     * to zero
     * 
     * @return the inserted, to the symbol table, new symbol, not null
     */
    expression::Symbol*     DefineNewSymbol(expression::ExprType type,
        const char* id, expression::Expression* index, unsigned int line);

    expression::Symbol* NewSymbol(expression::ExprType type, const char* id,
    expression::Expression* index, unsigned int line);
    /**
     * @brief Inserts a new anonymous function, parsed at a src line,
     * to the symbol table.
     * 
     * @param line the line on which the anonymous function is parsed,
     * greater or equal to zero
     * 
     * @return the inserted, to the symbol table, new anonymous function,
     * not null
     */
    expression::Symbol*     DefineNewAnonymousFunc(unsigned int line);
    /**
     * @brief Stashes a new formal argument, to be inserted
     *  to the symbol table later.
     * 
     * @param new_formal_arg the new formal argument to be stashed,
     * not null and of FORMAL_ARG scope space
     */
    void                    
        StashFormalArgument(expression::Symbol* new_formal_arg);
    /**
     * @brief Inserts the stashed formal arguments to the symbol table.
     * 
     */
    void                    DefineStashedFormalArguments();
    /**
     * @brief Checks wether a symbol is a global variable.
     * 
     * @param symbol the symbol to be checked, not null
     * 
     * @return wether a symbol is a global variable  
     */
    bool                    IsGlobalVar(expression::Symbol* symbol);
    /**
     * @brief Checks if a symbol is already defined at the current scope.
     * 
     * @param symbol the symbol to be checked, not null
     * 
     * @return  wether a symbol is already defined at the current scope
     */
    bool                    IsAtCurrentScope(expression::Symbol* symbol);
    }
}

#endif 