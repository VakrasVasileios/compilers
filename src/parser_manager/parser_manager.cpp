#include <string>
#include <list>
#include <iostream>
#include <assert.h>

#include "../../include/parser_manager/parser_manager.h"

#define LIB_FUNC_LINE  0
#define OUT_OF_SCOPE  -1

const unsigned int global_scope = 0;
unsigned int current_scope = OUT_OF_SCOPE;

bool method_call = false;

unsigned int loop_depth = 0;
unsigned int return_depth = 0;

unsigned int anonymus_funcs_counter = 0;

SymbolTable         symbol_table;

ProgramStack        program_stack;

std::vector<Quad>   quads;

std::list<FormalVariable*> stashed_formal_arguments;


bool IsLibraryFunction(Symbol* entry) {
    assert(entry != nullptr);
    return entry->get_type() == LIB_FUNC;
}

bool IsUserFunction(Symbol* entry) {
    assert(entry != nullptr);
    return entry->get_type() == USER_FUNC;
}

bool IsVariable(Symbol* entry) {
    assert(entry != nullptr);
    return entry->get_type() == VAR;
}

bool IsGlobalVar(Symbol* entry) {
    assert(entry != nullptr);
    return IsVariable(entry) && static_cast<Variable*>(entry)->get_space() == GLOBALVAR;
}

bool IsAtCurrentScope(Symbol* entry) {
    assert(entry != nullptr);
    return entry->get_scope() == current_scope;
}

bool ScopeIsGlobal() {
    return current_scope == global_scope;
}

bool IsMethodCall() {
    return method_call;
}

unsigned int GetLoopDepth() {
    return loop_depth;
}

unsigned int GetReturnDepth() {
    return return_depth;
}

Symbol* LookupGlobal(const char* name) {
    assert(name != nullptr);
    return program_stack.LookupGlobal(name);
}

Symbol* Lookup(const char* name) {
    assert(name != nullptr);
    return program_stack.Lookup(name);
}

Symbol* LookupFunc(const char* name) {
    assert(name != nullptr);
    return program_stack.LookupFunc(name);
}

void
Emit(Iopcode op, Expression* result, Expression* arg1, Expression* arg2, unsigned int line) {
    assert(result != nullptr);
    unsigned int label = quads.size() + 1;
    // if (op == JUMP)
    //     // put in stack
    // else if (op == IF_EQ)
    //     label = line + 2;
    // else
    //     label = line + 1;
    Quad q = quad(op, result, arg1, arg2, label, line);
    quads.push_back(q);
}

Symbol* InsertLocalVariable(const char* name, unsigned int line) {
    assert(name != nullptr);
    Symbol* entry = new LocalVariable(name, line, current_scope);
    program_stack.Top()->Insert(entry);

    return entry;
}
Symbol* InsertGlobalVariable(const char* name, unsigned int line) {
    assert(name != nullptr);
    Symbol* entry = new GlobalVariable(name, line, current_scope);
    program_stack.Top()->Insert(entry);
    
    return entry;
}

Symbol* InsertUserFunction(const char* name, unsigned int line) {
    assert(name != nullptr);
    Symbol* entry = new UserFunction(name, line, current_scope, stashed_formal_arguments);
    program_stack.Top()->Insert(entry);
    
    return entry;
}

Symbol* InsertUserFunction(unsigned int line) {
    std::string an = "$";
    an += std::to_string(++anonymus_funcs_counter);
    Symbol* entry = new UserFunction(an, line, current_scope, stashed_formal_arguments);
    program_stack.Top()->Insert(entry);
    
    return entry;
}

void InitLibraryFunctions() {  
    IncreaseScope(); 
    program_stack.Top()->Insert(new LibraryFunction("print", LIB_FUNC_LINE, global_scope));
    program_stack.Top()->Insert(new LibraryFunction("input", LIB_FUNC_LINE, global_scope));
    program_stack.Top()->Insert(new LibraryFunction("objectmemberkeys", LIB_FUNC_LINE, global_scope));
    program_stack.Top()->Insert(new LibraryFunction("objecttotalmembers", LIB_FUNC_LINE, global_scope));
    program_stack.Top()->Insert(new LibraryFunction("objectcopy", LIB_FUNC_LINE, global_scope));
    program_stack.Top()->Insert(new LibraryFunction("totalarguments", LIB_FUNC_LINE, global_scope));
    program_stack.Top()->Insert(new LibraryFunction("argument", LIB_FUNC_LINE, global_scope));
    program_stack.Top()->Insert(new LibraryFunction("typeof", LIB_FUNC_LINE, global_scope));
    program_stack.Top()->Insert(new LibraryFunction("strtonum", LIB_FUNC_LINE, global_scope));
    program_stack.Top()->Insert(new LibraryFunction("sqrt", LIB_FUNC_LINE, global_scope));
    program_stack.Top()->Insert(new LibraryFunction("cos", LIB_FUNC_LINE, global_scope));
    program_stack.Top()->Insert(new LibraryFunction("sin", LIB_FUNC_LINE, global_scope));
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

void SetMethodCall(bool methodcall) {
    ::method_call = method_call;
}

void IncreaseLoopDepth() {
    loop_depth++;
}

void DecreaseLoopDepth() {
    loop_depth--;
}

void IncreaseReturnDepth() {
    return_depth++;
}

void DecreaseReturnDepth() {
    return_depth--;
}

void HideLowerScopes() {
    if (current_scope != global_scope)
        program_stack.Top()->Deactivate();
    if (current_scope > 1)
        program_stack.DeactivateLowerScopes();
}

void EnableLowerScopes() {
    program_stack.ActivateLowerScopes();
}

void PushStashedFormalArguments(void) { 
    for (auto i : stashed_formal_arguments) {
        program_stack.Top()->Insert(i);
    }
    stashed_formal_arguments.clear();
}

bool
IsStashed(const char* name) {
    assert(name != nullptr);
    std::string wanted = name;
    for (auto i : stashed_formal_arguments) {
        if (i->get_id() == wanted) {
            return true;
        }
    }
    return false;
}

void StashFormalArgument(const char* name, unsigned int line) {
    assert(name != nullptr);
    if (!IsStashed(name))
        stashed_formal_arguments.push_back(new FormalVariable(name, line, current_scope + 1));
    else
        std::cout << "Error, formal argument " << name << " already declared, in line: " << line << std::endl;
}

void LogSymbolTable(std::ostream& output) {
    output << symbol_table;
}

void LogQuads(std::ostream& output) {
    for (auto quad : quads) {
        output << quad << std::endl;
    }
}


Function* GetPreviousFunction() {
    return program_stack.LookupPreviousFunc();
}