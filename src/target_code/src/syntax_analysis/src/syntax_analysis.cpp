#include <stack>
#include "../include/syntax_analysis.h"
#include "block.h"
#include "program_stack.h"
#include "symbol_table.h"

#define OUT_OF_SCOPE       -1
#define LIB_FUNC_LINE       0
#define TEMP_LINE           0
#define LIB_FUNC_OFFSET     0

namespace target_code
{
    namespace syntax_analysis 
    {
    const unsigned int          global_scope = 0;
    unsigned int                current_scope = OUT_OF_SCOPE;

    SymbolTable                 symbol_table;
    ProgramStack                program_stack;

    bool error_flag = false;

    bool NoErrorSignaled() {
        return error_flag == false;
    }  

    void SignalError(std::string msg, unsigned int line) {
        std::cout << "\033[31mError, in line: " << line <<
        ":\033[0m " << msg << std::endl;
        error_flag = 1;
    }

    void DefineSymbol(expression::Symbol* symbol) {
        assert(symbol != nullptr);
        program_stack.Top()->Insert(symbol);
    }

    void InitLibraryFunctions() {
        IncreaseScope(); 
        DefineSymbol(new expression::Symbol(expression::LIB_FUNC,
         "print", LIB_FUNC_LINE, global_scope, expression::PROGRAM_VAR,
          LIB_FUNC_OFFSET, nullptr));
        DefineSymbol(new expression::Symbol(expression::LIB_FUNC,
         "input", LIB_FUNC_LINE, global_scope, expression::PROGRAM_VAR,
          LIB_FUNC_OFFSET, nullptr));
        DefineSymbol(new expression::Symbol(expression::LIB_FUNC,
         "objectmemberkeys", LIB_FUNC_LINE, global_scope,
          expression::PROGRAM_VAR, LIB_FUNC_OFFSET, nullptr));
        DefineSymbol(new expression::Symbol(expression::LIB_FUNC,
         "objecttotalmembers", LIB_FUNC_LINE, global_scope,
          expression::PROGRAM_VAR, LIB_FUNC_OFFSET, nullptr));
        DefineSymbol(new expression::Symbol(expression::LIB_FUNC,
         "objectcopy", LIB_FUNC_LINE, global_scope, expression::PROGRAM_VAR,
          LIB_FUNC_OFFSET, nullptr));
        DefineSymbol(new expression::Symbol(expression::LIB_FUNC,
         "totalarguments", LIB_FUNC_LINE, global_scope,
          expression::PROGRAM_VAR, LIB_FUNC_OFFSET, nullptr));
        DefineSymbol(new expression::Symbol(expression::LIB_FUNC,
         "argument", LIB_FUNC_LINE, global_scope, expression::PROGRAM_VAR,
          LIB_FUNC_OFFSET, nullptr));
        DefineSymbol(new expression::Symbol(expression::LIB_FUNC,
         "typeof", LIB_FUNC_LINE, global_scope, expression::PROGRAM_VAR,
          LIB_FUNC_OFFSET, nullptr));
        DefineSymbol(new expression::Symbol(expression::LIB_FUNC,
         "strtonum", LIB_FUNC_LINE, global_scope, expression::PROGRAM_VAR,
          LIB_FUNC_OFFSET, nullptr));
        DefineSymbol(new expression::Symbol(expression::LIB_FUNC,
         "sqrt", LIB_FUNC_LINE, global_scope, expression::PROGRAM_VAR,
          LIB_FUNC_OFFSET, nullptr));
        DefineSymbol(new expression::Symbol(expression::LIB_FUNC, "cos",
         LIB_FUNC_LINE, global_scope, expression::PROGRAM_VAR, LIB_FUNC_OFFSET,
          nullptr));
        DefineSymbol(new expression::Symbol(expression::LIB_FUNC, "sin",
         LIB_FUNC_LINE, global_scope, expression::PROGRAM_VAR,
          LIB_FUNC_OFFSET, nullptr));
    }

    unsigned int CurrScope() {
        return current_scope;
    }

    void IncreaseScope() {
        Block* new_block = new Block();
        symbol_table.Insert(++current_scope, new_block);
        program_stack.Push(new_block);
    }

    void DecreaseScope() {
        program_stack.Top()->Deactivate();
        program_stack.Pop();
        --current_scope;
    }

    unsigned int program_var_offset       = 0;
    unsigned int function_local_offset    = 0;
    unsigned int formal_arg_offset        = 0;
    unsigned int scope_space_counter      = 1;

    std::stack<unsigned int> func_local_offset_stack;

    void
    StoreFuncLocalOffset(void) {
        func_local_offset_stack.push(function_local_offset);
    }

    void
    RestoreFuncLocalOffset(void) {
        PRECONDITION(!func_local_offset_stack.empty());
        function_local_offset = func_local_offset_stack.top();
        func_local_offset_stack.pop();
    }

    void
    ResetFuncLocalOffset(void) {
        function_local_offset = 0;
    }

    void
    ResetFormalArgOffset(void) {
        formal_arg_offset = 0;
    }

    expression::ScopeSpace
    CurrScopeSpace(void) {
        if (scope_space_counter == 1)
            return expression::PROGRAM_VAR;
        else if (scope_space_counter % 2 == 0)
            return expression::FORMAL_ARG;
        else
            return expression::FUNCTION_LOCAL;
    }

    unsigned int
    CurrScopeOffset(void) {
        switch (CurrScopeSpace()) {
            case expression::PROGRAM_VAR:
                return program_var_offset;
            case expression::FUNCTION_LOCAL:
                return function_local_offset;
            case expression::FORMAL_ARG:
                return formal_arg_offset;        
            default:
                assert(false);
        }
    }

    void
    IncreaseCurrOffset(void) {
        switch (CurrScopeSpace()) {
            case expression::PROGRAM_VAR    : ++program_var_offset; break;
            case expression::FUNCTION_LOCAL : ++function_local_offset; break;
            case expression::FORMAL_ARG     : ++formal_arg_offset; break;
            default:
                assert(false);
        }
    }

    void
    EnterScopeSpace(void) {
        ++scope_space_counter;
    }

    void
    ExitScopeSpace(void) {
        PRECONDITION(scope_space_counter > 1);
        --scope_space_counter;
    }

    void EnableLowerScopes() {
        program_stack.ActivateLowerScopes();
    }

    void HideLowerScopes() {
        if (current_scope != global_scope)
            program_stack.Top()->Deactivate();
        if (current_scope > 1)
            program_stack.DeactivateLowerScopes();
    }

    expression::Symbol* Lookup(const char* id) {
        PRECONDITION(id != nullptr);
        return program_stack.Lookup(id);
    }

    expression::Symbol* LookupGlobal(const char* id) {
        PRECONDITION(id != nullptr);
        return program_stack.LookupGlobal(id);
    }

    expression::Symbol* NewSymbol(expression::ExprType type, const char* id,
        expression::Expression* index, unsigned int line) {
        assert (id != nullptr);
        return new expression::Symbol(type, id, line, current_scope,
            CurrScopeSpace(), CurrScopeOffset(), index);
    }

    expression::Symbol* DefineNewSymbol(expression::ExprType type,
        const char* id, expression::Expression* index, unsigned int line) {
        PRECONDITION(id != nullptr);
        auto new_symbol = NewSymbol(type, id, index, line);
        IncreaseCurrOffset();
        DefineSymbol(new_symbol);

        return new_symbol;
    }

    unsigned int anonymus_funcs_counter = 0;

    std::string NewAnonymousFuncName() {
        std::string an = "$";
        an += std::to_string(anonymus_funcs_counter++);

        return an;
    }

    expression::Symbol* NewAnonymousFunc(unsigned int line) {
        assert(line >= 0);
        return NewSymbol(expression::USER_FUNC, NewAnonymousFuncName().c_str(),
            nullptr, line);
    }

    expression::Symbol* DefineNewAnonymousFunc(unsigned int line) {
        PRECONDITION(line >= 0);
        auto new_an_func = NewAnonymousFunc(line);
        IncreaseCurrOffset();
        DefineSymbol(new_an_func);

        return new_an_func;
    }

    std::list<expression::Symbol*>  stashed_formal_arguments;

    void StashFormalArgument(expression::Symbol* new_formal_arg) {
        PRECONDITION(new_formal_arg != nullptr);
        PRECONDITION(new_formal_arg->get_space() == expression::FORMAL_ARG);
        stashed_formal_arguments.push_back(new_formal_arg);       
    }

    void DefineStashedFormalArguments() {
        for (auto i : stashed_formal_arguments) 
            DefineSymbol(i);
        stashed_formal_arguments.clear();
    }

    bool IsGlobalVar(expression::Symbol* symbol) {
        PRECONDITION(symbol != nullptr);
        return IsVariable(symbol) && symbol->get_scope() == global_scope; 
    }

    bool IsAtCurrentScope(expression::Symbol* symbol) {
        PRECONDITION(symbol != nullptr);
        return symbol->get_scope() == current_scope;
    }

    void LogSymTable(std::ostream& output) {
        output << symbol_table;
    }
    }
} 