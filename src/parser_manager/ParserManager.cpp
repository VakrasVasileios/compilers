#include "../../include/parser_manager/ParserManager.h"
#include <string>
#include <list>
#include <iostream>

#define LIB_FUNC_LINE  0
#define OUT_OF_SCOPE  -1

const unsigned int global_scope = 0;
unsigned int current_scope = OUT_OF_SCOPE;

bool methodCall = false;
bool validReturn = false;

unsigned int loopDepth = 0;

unsigned int anonymusFuncsCounter = 0;

SymbolTable symbolTable;

ProgramStack  programStack;

std::list<FormalVariableEntry> stashedFormalArguments;

void init_library_functions() {  
    increase_scope(); 
    programStack.top()->insert(new LibraryFunctionEntry("print", LIB_FUNC_LINE, global_scope));
    programStack.top()->insert(new LibraryFunctionEntry("input", LIB_FUNC_LINE, global_scope));
    programStack.top()->insert(new LibraryFunctionEntry("objectmemberkeys", LIB_FUNC_LINE, global_scope));
    programStack.top()->insert(new LibraryFunctionEntry("objecttotalmembers", LIB_FUNC_LINE, global_scope));
    programStack.top()->insert(new LibraryFunctionEntry("objectcopy", LIB_FUNC_LINE, global_scope));
    programStack.top()->insert(new LibraryFunctionEntry("totalarguments", LIB_FUNC_LINE, global_scope));
    programStack.top()->insert(new LibraryFunctionEntry("argument", LIB_FUNC_LINE, global_scope));
    programStack.top()->insert(new LibraryFunctionEntry("typeof", LIB_FUNC_LINE, global_scope));
    programStack.top()->insert(new LibraryFunctionEntry("strtonum", LIB_FUNC_LINE, global_scope));
    programStack.top()->insert(new LibraryFunctionEntry("sqrt", LIB_FUNC_LINE, global_scope));
    programStack.top()->insert(new LibraryFunctionEntry("cos", LIB_FUNC_LINE, global_scope));
    programStack.top()->insert(new LibraryFunctionEntry("sin", LIB_FUNC_LINE, global_scope));
}

void increase_scope() {
    Block* newBlock = new Block();
    symbolTable.insert(++current_scope, newBlock);
    programStack.push(newBlock);
}

void decrease_scope() {
    programStack.pop();
    --current_scope;
}

void set_method_call(bool methodcall) {
    ::methodCall = methodCall;
}

bool is_method_call() {
    return methodCall;
}

void  set_valid_return(bool _validReturn) {
    validReturn = _validReturn;
}

bool is_valid_return(void) {
    return validReturn;
}

void increase_loop_depth() {
    loopDepth++;
}

void decrease_loop_depth() {
    loopDepth--;
}

unsigned int get_loop_depth() {
    return loopDepth;
}

void hide_lower_scopes() {
    programStack.deactivateLowerScopes();
}

void enable_lower_scopes() {
    programStack.activateLowerScopes();
}

bool scope_is_global() {
    return current_scope == global_scope;
}

SymbolTableEntry*   LookupGlobal(const char* name) {
    return programStack.lookupGlobal(name);
}

SymbolTableEntry* lookup(const char* name) {
    return programStack.lookup(name);
}

bool is_library_function(SymbolTableEntry* entry) {
    return entry != nullptr && entry->getType() == LIB_FUNC;
}

bool is_user_function(SymbolTableEntry* entry) {
    return entry != nullptr && entry->getType() == USER_FUNC;
}

bool is_formal_variable(SymbolTableEntry* entry) {
    return entry != nullptr && entry->getType() == FORMAL_VAR;
}

bool is_global_variable(SymbolTableEntry* entry) {
    return entry != nullptr && entry->getType() == GLOBAL_VAR;
} 

bool is_local_variable(SymbolTableEntry* entry) {
    return entry != nullptr && entry->getType() == LOCAL_VAR;
}

void insert_user_function(const char* name, unsigned int line) {
    programStack.top()->insert(new UserFunctionEntry(name, line, current_scope, stashedFormalArguments)); 
}

void insert_variable_local(const char* name, unsigned int line) {
    programStack.top()->insert(new LocalVariableEntry(name, line, current_scope)); 
}
void insert_variable_global(const char* name, unsigned int line) {
    programStack.top()->insert(new GlobalVariableEntry(name, line, current_scope)); 
}

void insert_user_function(unsigned int line) {
    std::string an = "$";
    an += anonymusFuncsCounter;
    programStack.top()->insert(new UserFunctionEntry(an, line, current_scope, stashedFormalArguments)); 
}

void push_stashed_formal_arguments(void) { 
    for (auto i : stashedFormalArguments) {
        programStack.top()->insert(&i);
    }
    stashedFormalArguments.clear();
}

void stash_formal_argument(const char* name, unsigned int line) {
    stashedFormalArguments.push_back(FormalVariableEntry(name, line, current_scope + 1));
}

void log_symbol_table(std::ostream& output) {
    output << symbolTable;
}