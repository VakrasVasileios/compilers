#include "../../../include/target_code/intermediate_code/syntax_analysis/syntax_analysis.h"

#define OUT_OF_SCOPE       -1
#define LIB_FUNC_LINE       0
#define TEMP_LINE           0

namespace syntax_analysis {
    const unsigned int          global_scope = 0;
    unsigned int                current_scope = OUT_OF_SCOPE;
    unsigned int                program_var_offset = 0;

    SymbolTable                 symbol_table;
    ProgramStack                program_stack;

    bool error_flag = false;

    inline bool NoErrorSignaled() {
        return error_flag == false;
    }  

    void SignalError(std::string msg, unsigned int line) {
        #if !defined TEST
            std::cout << "\033[31mError, in line: " << line << ":\033[0m " << msg << std::endl;
        #else
            std::cout << "Error, in line: " << yylineno << ": " << msg << std::endl; 
        #endif    
        error_flag = 1;
    }

    void InitLibraryFunctions() {
        IncreaseScope(); 
        DefineSymbol(new expression::Symbol(expression::LIB_FUNC, "print", LIB_FUNC_LINE, global_scope, expression::PROGRAM_VAR, program_var_offset++, nullptr));
        DefineSymbol(new expression::Symbol(expression::LIB_FUNC, "input", LIB_FUNC_LINE, global_scope, expression::PROGRAM_VAR, program_var_offset++, nullptr));
        DefineSymbol(new expression::Symbol(expression::LIB_FUNC, "objectmemberkeys", LIB_FUNC_LINE, global_scope, expression::PROGRAM_VAR, program_var_offset++, nullptr));
        DefineSymbol(new expression::Symbol(expression::LIB_FUNC, "objecttotalmembers", LIB_FUNC_LINE, global_scope, expression::PROGRAM_VAR, program_var_offset++, nullptr));
        DefineSymbol(new expression::Symbol(expression::LIB_FUNC, "objectcopy", LIB_FUNC_LINE, global_scope, expression::PROGRAM_VAR, program_var_offset++, nullptr));
        DefineSymbol(new expression::Symbol(expression::LIB_FUNC, "totalarguments", LIB_FUNC_LINE, global_scope, expression::PROGRAM_VAR, program_var_offset++, nullptr));
        DefineSymbol(new expression::Symbol(expression::LIB_FUNC, "argument", LIB_FUNC_LINE, global_scope, expression::PROGRAM_VAR, program_var_offset++, nullptr));
        DefineSymbol(new expression::Symbol(expression::LIB_FUNC, "typeof", LIB_FUNC_LINE, global_scope, expression::PROGRAM_VAR, program_var_offset++, nullptr));
        DefineSymbol(new expression::Symbol(expression::LIB_FUNC, "strtonum", LIB_FUNC_LINE, global_scope, expression::PROGRAM_VAR,program_var_offset++, nullptr));
        DefineSymbol(new expression::Symbol(expression::LIB_FUNC, "sqrt", LIB_FUNC_LINE, global_scope, expression::PROGRAM_VAR, program_var_offset++, nullptr));
        DefineSymbol(new expression::Symbol(expression::LIB_FUNC, "cos", LIB_FUNC_LINE, global_scope, expression::PROGRAM_VAR, program_var_offset++, nullptr));
        DefineSymbol(new expression::Symbol(expression::LIB_FUNC, "sin", LIB_FUNC_LINE, global_scope, expression::PROGRAM_VAR, program_var_offset++, nullptr));
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

    unsigned int programVarOffset       = 0;
    unsigned int functionLocalOffset    = 0;
    unsigned int formalArgOffset        = 0;
    unsigned int scopeSpaceCounter      = 1;

    std::stack<unsigned int> funcLocalOffset_stack;
    std::stack<unsigned int> formalOffset_stack;

    void
    store_funclocal_offset(void) {
        funcLocalOffset_stack.push(functionLocalOffset);
    }

    void
    restore_funclocal_offset(void) {
        assert(!funcLocalOffset_stack.empty());
        functionLocalOffset = funcLocalOffset_stack.top();
        funcLocalOffset_stack.pop();
    }

    void
    reset_funclocal_offset(void) {
        functionLocalOffset = 0;
    }

    void
    reset_formalarg_offset(void) {
        formalArgOffset = 0;
    }

    expression::ScopeSpace
    curr_scope_space(void) {
        if (scopeSpaceCounter == 1)
            return expression::PROGRAM_VAR;
        else if (scopeSpaceCounter % 2 == 0)
            return expression::FORMAL_ARG;
        else
            return expression::FUNCTION_LOCAL;
    }

    unsigned int
    curr_scope_offset(void) {
        switch (curr_scope_space()) {
            case expression::PROGRAM_VAR:
                return programVarOffset;
            case expression::FUNCTION_LOCAL:
                return functionLocalOffset;
            case expression::FORMAL_ARG:
                return formalArgOffset;        
            default:
                assert(false);
        }
    }

    void
    increase_curr_offset(void) {
        switch (curr_scope_space()) {
            case expression::PROGRAM_VAR    : ++programVarOffset; break;
            case expression::FUNCTION_LOCAL : ++functionLocalOffset; break;
            case expression::FORMAL_ARG     : ++formalArgOffset; break;
            default:
                assert(false);
        }
    }

    void
    enter_scope_space(void) {
        ++scopeSpaceCounter;
    }

    void
    exit_scope_space(void) {
        assert(scopeSpaceCounter > 1);
        --scopeSpaceCounter;
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

    void DefineSymbol(expression::Symbol* symbol) {
        assert(symbol != nullptr);
        program_stack.Top()->Insert(symbol);
    }

    expression::Symbol* NewSymbol(expression::ExprType type, const char* id, expression::Expression* index, unsigned int line) {
        assert (id != nullptr);
        return new expression::Symbol(type, id, line, current_scope, curr_scope_space(), curr_scope_offset(), index);
    }

    expression::Symbol* DefineNewSymbol(expression::ExprType type, const char* id, expression::Expression* index, unsigned int line) {
        assert(id != nullptr);
        auto new_symbol = NewSymbol(type, id, index, line);
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
        return NewSymbol(expression::USER_FUNC, NewAnonymousFuncName().c_str(), nullptr, line);
    }

    expression::Symbol* DefineNewAnonymousFunc(unsigned int line) {
        auto new_an_func = NewAnonymousFunc(line);
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

    inline bool IsGlobalVar(expression::Symbol* symbol) {
        return IsVariable(symbol) && symbol->get_scope() == global_scope; 
    }

    inline bool IsAtCurrentScope(expression::Symbol* symbol) {
        return symbol->get_scope() == current_scope;
    }

    inline void LogSymTable(std::ostream& output) {
        output << symbol_table;
    }
} 