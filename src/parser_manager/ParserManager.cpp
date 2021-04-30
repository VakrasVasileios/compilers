#include "../../include/parser_manager/ParserManager.h"
#include "../../include/symbol_table/SymbolTable.h"
#include "../../include/symbol_table/symbol_table_entry/SymbolTableEntry.h"
#include "../../include/block_stack/BlockStack.h"
#include "../../include/symbol_table/symbol_table_entry/variable_entry/FormalVariableEntry.h"
#include <string>
#include <list>
#include <iostream>

#define LIB_FUNC_LINE -1

const unsigned int global_scope = 0;
unsigned int current_scope = 0;

bool methodCall = false;
bool validReturn = false;

unsigned int loopDepth = 0;

unsigned int anonymusFuncsCounter = 0;

SymbolTable symbolTable;

BlockStack  blockStack;

std::list<FormalVariableEntry> stashedFormalArguments;

void init_library_functions() {
    symbolTable.insert(current_scope, Block());
    blockStack.pushBlock(getCurrent_Block());
    blockStack.topBlock()->addSymbolTableEntry(LibraryFunctionEntry("print", LIB_FUNC_LINE, global_scope));
    blockStack.topBlock()->addSymbolTableEntry(LibraryFunctionEntry("input", LIB_FUNC_LINE, global_scope));
    blockStack.topBlock()->addSymbolTableEntry(LibraryFunctionEntry("objectmemberkeys", LIB_FUNC_LINE, global_scope));
    blockStack.topBlock()->addSymbolTableEntry(LibraryFunctionEntry("objecttotalmembers", LIB_FUNC_LINE, global_scope));
    blockStack.topBlock()->addSymbolTableEntry(LibraryFunctionEntry("objectcopy", LIB_FUNC_LINE, global_scope));
    blockStack.topBlock()->addSymbolTableEntry(LibraryFunctionEntry("totalarguments", LIB_FUNC_LINE, global_scope));
    blockStack.topBlock()->addSymbolTableEntry(LibraryFunctionEntry("argument", LIB_FUNC_LINE, global_scope));
    blockStack.topBlock()->addSymbolTableEntry(LibraryFunctionEntry("typeof", LIB_FUNC_LINE, global_scope));
    blockStack.topBlock()->addSymbolTableEntry(LibraryFunctionEntry("strtonum", LIB_FUNC_LINE, global_scope));
    blockStack.topBlock()->addSymbolTableEntry(LibraryFunctionEntry("sqrt", LIB_FUNC_LINE, global_scope));
    blockStack.topBlock()->addSymbolTableEntry(LibraryFunctionEntry("cos", LIB_FUNC_LINE, global_scope));
    blockStack.topBlock()->addSymbolTableEntry(LibraryFunctionEntry("sin", LIB_FUNC_LINE, global_scope));
}

Block* getCurrent_Block() {
    return &*(--symbolTable.getBlocksTable(current_scope).end()); //end() points to nullptr
}

void increase_scope() {
    current_scope++;
    symbolTable.insert(current_scope, Block());
    blockStack.pushBlock(getCurrent_Block());
}

void decrease_scope() {
    blockStack.popBlock();
    --current_scope;
}

void set_method_call(bool methodcall) {
    ::methodCall = methodCall;
}

bool is_method_call() {
    return methodCall;
}

void set_valid_return(bool _validReturn) {
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
    blockStack.hideLowerScopes();
}

void enable_lower_scopes() {
    blockStack.enableLowerScopes();
}

bool lookup_library_function(const char* name) {
    SymbolTableEntry *r_value = blockStack.Lookup(std::string(name));
    if(r_value == nullptr || r_value->getType() != LIB_FUNC)
        return false;
    else
        return true;
}

bool lookup_user_function(const char* name) {
    SymbolTableEntry *r_value = blockStack.Lookup(std::string(name));
    if(r_value == nullptr || r_value->getType() != USER_FUNC)
        return false;
    else
        return true;
}

bool lookup_local_variable(const char* name) {
    SymbolTableEntry *r_value = blockStack.Lookup(std::string(name));
    if(r_value == nullptr || r_value->getType() != LOCAL_VAR)
        return false;
    else
        return true;
}

bool lookup_global_variable(const char* name) {
    SymbolTableEntry *r_value = blockStack.Lookup(std::string(name));
    if(r_value == nullptr)
        return false;
    else
        return true;
}

bool lookup_formal_variable(const char* name) {
    SymbolTableEntry *r_value = blockStack.Lookup(std::string(name));
    if(r_value == nullptr || r_value->getType() != FORMAL_VAR)
        return false;
    else
        return true;
}

void insert_user_function(const char* name, unsigned int line) {
    blockStack.topBlock()->addSymbolTableEntry(UserFunctionEntry(std::string(name), line, current_scope, stashedFormalArguments);
}

void insert_user_function(unsigned int line) {
    std::string an = "$";
    an += anonymusFuncsCounter;
    blockStack.topBlock()->addSymbolTableEntry(UserFunctionEntry(an, line, current_scope, stashedFormalArguments);
}

void push_stashed_lvalues() { //TODO

}

void stash_lvalue(const char* name, unsigned int line) { //TODO

}

void reset_lvalues_stash() { //TODO

}

void push_stashed_formal_arguments(void) {
    for (auto i : stashedFormalArguments) {
        blockStack.topBlock()->addSymbolTableEntry(i);
    }
    stashedFormalArguments.clear();
}

void stash_formal_argument(const char* name, unsigned int line) {
    stashedFormalArguments.push_back(FormalVariableEntry(std::string(name), line, current_scope + 1));
}

void log_symbol_table() {
    std::cout << symbolTable << std::endl;
}