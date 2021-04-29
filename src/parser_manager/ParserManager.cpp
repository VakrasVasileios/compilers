#include "../../include/parser_manager/ParserManager.h"
#include "../../include/symbol_table/SymbolTable.h"
#include "../../include/id/Function.h"
#include "../../include/symbol_table/symbol_table_entry/SymbolTableEntry.h"
#include "../../include/block_stack/BlockStack.h"

#define LIB_FUNC_LINE -1

const unsigned int global_scope = 0;
unsigned int current_scope = 0;

bool methodCall = false;

unsigned int loopDepth = 0;

unsigned int anonymusFuncsCounter = 0;

SymbolTable symbolTable;

BlockStack  blockStack;

void init_library_functions() {
    symbolTable.insert(current_scope, Block());
    blockStack.pushBlock(getCurrent_Block());
    blockStack.topBlock()->addSymbolTableEntry(LibraryFunctionEntry(Function("print", LIB_FUNC_LINE, global_scope)));
    blockStack.topBlock()->addSymbolTableEntry(LibraryFunctionEntry(Function("input", LIB_FUNC_LINE, global_scope)));
    blockStack.topBlock()->addSymbolTableEntry(LibraryFunctionEntry(Function("objectmemberkeys", LIB_FUNC_LINE, global_scope)));
    blockStack.topBlock()->addSymbolTableEntry(LibraryFunctionEntry(Function("objecttotalmembers", LIB_FUNC_LINE, global_scope)));
    blockStack.topBlock()->addSymbolTableEntry(LibraryFunctionEntry(Function("objectcopy", LIB_FUNC_LINE, global_scope)));
    blockStack.topBlock()->addSymbolTableEntry(LibraryFunctionEntry(Function("totalarguments", LIB_FUNC_LINE, global_scope)));
    blockStack.topBlock()->addSymbolTableEntry(LibraryFunctionEntry(Function("argument", LIB_FUNC_LINE, global_scope)));
    blockStack.topBlock()->addSymbolTableEntry(LibraryFunctionEntry(Function("typeof", LIB_FUNC_LINE, global_scope)));
    blockStack.topBlock()->addSymbolTableEntry(LibraryFunctionEntry(Function("strtonum", LIB_FUNC_LINE, global_scope)));
    blockStack.topBlock()->addSymbolTableEntry(LibraryFunctionEntry(Function("sqrt", LIB_FUNC_LINE, global_scope)));
    blockStack.topBlock()->addSymbolTableEntry(LibraryFunctionEntry(Function("cos", LIB_FUNC_LINE, global_scope)));
    blockStack.topBlock()->addSymbolTableEntry(LibraryFunctionEntry(Function("sin", LIB_FUNC_LINE, global_scope)));
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
    SymbolTableEntry *r_value = blockStack.Lookup(LIB_FUNC, std::string(name))
    if(r_value == nullptr)
        return false;
    else
        return true;
}

bool lookup_user_function(const char* name) {
    SymbolTableEntry *r_value = blockStack.Lookup(USER_FUNC, std::string(name))
    if(r_value == nullptr)
        return false;
    else
        return true;
}

bool lookup_local_variable(const char* name) {
    SymbolTableEntry *r_value = blockStack.Lookup(LOCAL_VAR, std::string(name))
    if(r_value == nullptr)
        return false;
    else
        return true;
}

bool lookup_global_variable(const char* name) {
    SymbolTableEntry *r_value = blockStack.Lookup(GLOBAL_VAR, std::string(name))
    if(r_value == nullptr)
        return false;
    else
        return true;
}

bool lookup_formal_variable(const char* name) {
    SymbolTableEntry *r_value = blockStack.Lookup(FORMAL_VAR, std::string(name))
    if(r_value == nullptr)
        return false;
    else
        return true;
}

void insert_user_function(const char* name, unsigned int line) {
    blockStack.topBlock()->addSymbolTableEntry(UserFunctionEntry(Function(std::string(name), line, global_scope))));
}

void insert_user_function(unsigned int line) {
    std::string an = "$";
    an += anonymusFuncsCounter;
    blockStack.topBlock()->addSymbolTableEntry(UserFunctionEntry(Function(an, line, global_scope));
}

void push_stashed_lvalues() { //TODO

}

void stash_lvalue(const char* name, unsigned int line) { //TODO

}

void reset_lvalues_stash() { //TODO

}

void stash_formal_argument(const char* name, unsigned int line) { //TODO

}

void log_symbol_table() { //TODO

}