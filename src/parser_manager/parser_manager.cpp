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

SymbolTable symbol_table;

ProgramStack  program_stack;

std::list<FormalVariableEntry*> stashed_formal_arguments;

bool IsLibraryFunction(SymbolTableEntry* entry) {
    assert(entry != nullptr);
    return entry->get_type() == LIB_FUNC;
}

bool IsUserFunction(SymbolTableEntry* entry) {
    assert(entry != nullptr);
    return entry->get_type() == USER_FUNC;
}

bool IsVariable(SymbolTableEntry* entry) {
    assert(entry != nullptr);
    return entry->get_type() == VAR;
}

bool IsGlobalVar(SymbolTableEntry* entry) {
    assert(entry != nullptr);
    return IsVariable(entry) && static_cast<VariableEntry*>(entry)->get_scope_type() == GLOBAL_T;
}

bool IsAtCurrentScope(SymbolTableEntry* entry) {
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

SymbolTableEntry* LookupGlobal(const char* name) {
    assert(name != nullptr);
    return program_stack.LookupGlobal(name);
}

SymbolTableEntry* Lookup(const char* name) {
    assert(name != nullptr);
    return program_stack.Lookup(name);
}

SymbolTableEntry* LookupFunc(const char* name) {
    assert(name != nullptr);
    return program_stack.LookupFunc(name);
}

SymbolTableEntry* InsertLocalVariable(const char* name, unsigned int line) {
    assert(name != nullptr);
    SymbolTableEntry* entry = new LocalVariableEntry(name, line, current_scope);
    program_stack.Top()->Insert(entry);

    return entry;
}
SymbolTableEntry* InsertGlobalVariable(const char* name, unsigned int line) {
    assert(name != nullptr);
    SymbolTableEntry* entry = new GlobalVariableEntry(name, line, current_scope);
    program_stack.Top()->Insert(entry);
    
    return entry;
}

SymbolTableEntry* InsertUserFunction(const char* name, unsigned int line) {
    assert(name != nullptr);
    SymbolTableEntry* entry = new UserFunctionEntry(name, line, current_scope, stashed_formal_arguments);
    program_stack.Top()->Insert(entry);
    
    return entry;
}

SymbolTableEntry* InsertUserFunction(unsigned int line) {
    std::string an = "$";
    an += std::to_string(++anonymus_funcs_counter);
    SymbolTableEntry* entry = new UserFunctionEntry(an, line, current_scope, stashed_formal_arguments);
    program_stack.Top()->Insert(entry);
    
    return entry;
}

void InitLibraryFunctions() {  
    IncreaseScope(); 
    program_stack.Top()->Insert(new LibraryFunctionEntry("print", LIB_FUNC_LINE, global_scope));
    program_stack.Top()->Insert(new LibraryFunctionEntry("input", LIB_FUNC_LINE, global_scope));
    program_stack.Top()->Insert(new LibraryFunctionEntry("objectmemberkeys", LIB_FUNC_LINE, global_scope));
    program_stack.Top()->Insert(new LibraryFunctionEntry("objecttotalmembers", LIB_FUNC_LINE, global_scope));
    program_stack.Top()->Insert(new LibraryFunctionEntry("objectcopy", LIB_FUNC_LINE, global_scope));
    program_stack.Top()->Insert(new LibraryFunctionEntry("totalarguments", LIB_FUNC_LINE, global_scope));
    program_stack.Top()->Insert(new LibraryFunctionEntry("argument", LIB_FUNC_LINE, global_scope));
    program_stack.Top()->Insert(new LibraryFunctionEntry("typeof", LIB_FUNC_LINE, global_scope));
    program_stack.Top()->Insert(new LibraryFunctionEntry("strtonum", LIB_FUNC_LINE, global_scope));
    program_stack.Top()->Insert(new LibraryFunctionEntry("sqrt", LIB_FUNC_LINE, global_scope));
    program_stack.Top()->Insert(new LibraryFunctionEntry("cos", LIB_FUNC_LINE, global_scope));
    program_stack.Top()->Insert(new LibraryFunctionEntry("sin", LIB_FUNC_LINE, global_scope));
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
        stashed_formal_arguments.push_back(new FormalVariableEntry(name, line, current_scope + 1));
    else
        std::cout << "Error, formal argument " << name << " already declared, in line: " << line << std::endl;
}

void LogSymbolTable(std::ostream& output) {
    output << symbol_table;
}