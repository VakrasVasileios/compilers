#include "../../include/parser_manager/parser_manager.h"
#include <string>
#include <list>
#include <iostream>

#define LIB_FUNC_LINE  0
#define OUT_OF_SCOPE  -1

const unsigned int global_scope = 0;
unsigned int current_scope = OUT_OF_SCOPE;

bool method_call = false;
bool valid_return = false;

unsigned int loop_depth = 0;

unsigned int anonymus_funcs_counter = 0;

SymbolTable symbol_table;

ProgramStack  program_stack;

std::list<FormalVariableEntry*> stashed_formal_arguments;

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

bool IsMethodCall() {
    return method_call;
}

void  SetValidReturn(bool valid_return) {
    ::valid_return = valid_return;
}

bool IsValidReturn(void) {
    return valid_return;
}

void IncreaseLoopDepth() {
    loop_depth++;
}

void DecreaseLoopDepth() {
    loop_depth--;
}

unsigned int GetLoopDepth() {
    return loop_depth;
}

void HideLowerScopes() {
    program_stack.DeactivateLowerScopes();
}

void EnableLowerScopes() {
    program_stack.ActivateLowerScopes();
}

bool ScopeIsGlobal() {
    return current_scope == global_scope;
}

SymbolTableEntry*   LookupGlobal(const char* name) {
    return program_stack.LookupGlobal(name);
}

SymbolTableEntry* Lookup(const char* name) {
    return program_stack.Lookup(name);
}

bool IsLibraryFunction(SymbolTableEntry* entry) {
    return entry != nullptr && entry->get_type() == LIB_FUNC;
}

bool IsUserFunction(SymbolTableEntry* entry) {
    return entry != nullptr && entry->get_type() == USER_FUNC;
}

bool IsFormalVariable(SymbolTableEntry* entry) {
    return entry != nullptr && entry->get_type() == FORMAL_VAR;
}

bool IsGlobalVariable(SymbolTableEntry* entry) {
    return entry != nullptr && entry->get_type() == GLOBAL_VAR;
} 

bool IsLocalVariable(SymbolTableEntry* entry) {
    return entry != nullptr && entry->get_type() == LOCAL_VAR;
}

void InsertUserFunction(const char* name, unsigned int line) {
    program_stack.Top()->Insert(new UserFunctionEntry(name, line, current_scope, stashed_formal_arguments)); 
}

void InsertLocalVariable(const char* name, unsigned int line) {
    program_stack.Top()->Insert(new LocalVariableEntry(name, line, current_scope)); 
}
void InsertGlobalVariable(const char* name, unsigned int line) {
    program_stack.Top()->Insert(new GlobalVariableEntry(name, line, current_scope)); 
}

void InsertUserFunction(unsigned int line) {
    std::string an = "$";
    an += anonymus_funcs_counter;
    program_stack.Top()->Insert(new UserFunctionEntry(an, line, current_scope, stashed_formal_arguments)); 
}

void PushStashedFormalArguments(void) { 
    for (auto i : stashed_formal_arguments) {
        program_stack.Top()->Insert(i);
    }
    stashed_formal_arguments.clear();
}

void StashFormalArgument(const char* name, unsigned int line) {
    stashed_formal_arguments.push_back(new FormalVariableEntry(name, line, current_scope + 1));
}

void LogSymbolTable(std::ostream& output) {
    output << symbol_table;
}