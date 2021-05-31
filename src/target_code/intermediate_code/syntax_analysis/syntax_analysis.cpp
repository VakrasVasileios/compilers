#include "../../include/intermediate_code/syntax_analysis.h"

#define OUT_OF_SCOPE       -1
#define LIB_FUNC_LINE       0
#define TEMP_LINE           0

namespace target_code {
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
        if (InFuncDef()) {
            auto new_symbol = new expression::Symbol(type, id, line, current_scope, expression::FUNCTION_LOCAL, func_def_stmts.top()->get_offset(), index);
            func_def_stmts.top()->IncreaseOffset();
            
            return new_symbol;
        }
        else {
            return new expression::Symbol(type, id, line, current_scope, expression::PROGRAM_VAR, program_var_offset++, index);
        }
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

    void StashFormalArgument(const char* id, unsigned int line) {
        PRECONDITION(id != nullptr);
        auto top_func_def_stmt = func_def_stmts.top();
        auto func = top_func_def_stmt->get_sym();
        auto offset = func->get_formal_arguments().size();

        auto new_formal_arg = new expression::Symbol(expression::VAR, id, line, current_scope + 1, expression::FORMAL_ARG, offset, nullptr);
        if (func->HasFormalArg(new_formal_arg)) {
            SignalError("formal argument " + std::string(id) + " already declared", line);
        }
        else {
            func->AddFormalArg(new_formal_arg);
            stashed_formal_arguments.push_back(new_formal_arg);       
        }
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