#include "../../include/parser_manager/ParserManager.h"
#include "../../include/symbol_table/SymbolTable.h"
#include "../../include/id/Function.h"

#define LIB_FUNC_LINE -1

const unsigned int global_scope = 0;
unsigned int current_scope = 0;

bool methodCall = false;

unsigned int loopDepth = 0;

SymbolTable symbolTable;

void insert_library_functions() {
    symbolTable.insert(global_scope, LibraryFunctionEntry(Function("print", LIB_FUNC_LINE, global_scope)));
    symbolTable.insert(global_scope, LibraryFunctionEntry(Function("input", LIB_FUNC_LINE, global_scope)));
    symbolTable.insert(global_scope, LibraryFunctionEntry(Function("objectmemberkeys", LIB_FUNC_LINE, global_scope)));
    symbolTable.insert(global_scope, LibraryFunctionEntry(Function("objecttotalmembers", LIB_FUNC_LINE, global_scope)));
    symbolTable.insert(global_scope, LibraryFunctionEntry(Function("objectcopy", LIB_FUNC_LINE, global_scope)));
    symbolTable.insert(global_scope, LibraryFunctionEntry(Function("totalarguments", LIB_FUNC_LINE, global_scope)));
    symbolTable.insert(global_scope, LibraryFunctionEntry(Function("argument", LIB_FUNC_LINE, global_scope)));
    symbolTable.insert(global_scope, LibraryFunctionEntry(Function("typeof", LIB_FUNC_LINE, global_scope)));
    symbolTable.insert(global_scope, LibraryFunctionEntry(Function("strtonum", LIB_FUNC_LINE, global_scope)));
    symbolTable.insert(global_scope, LibraryFunctionEntry(Function("sqrt", LIB_FUNC_LINE, global_scope)));
    symbolTable.insert(global_scope, LibraryFunctionEntry(Function("cos", LIB_FUNC_LINE, global_scope)));
    symbolTable.insert(global_scope, LibraryFunctionEntry(Function("sin", LIB_FUNC_LINE, global_scope)));
}

void increase_scope() { //TODO

}

void decrease_scope() { //TODO

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

void hide_lower_scopes() { //TODO

}

void enable_lower_scopes() { //TODO

}

bool lookup_library_function(const char* name) { //TODO

}

bool lookup_user_function(const char* name) { //TODO

}

bool lookup_local_variable(const char* name) { //TODO

}

bool lookup_global_variable(const char* name) { //TODO

}

bool lookup_formal_variable(const char* name) { //TODO

}

void insert_user_function(const char* name, unsigned int line) { //TODO

}

void insert_user_function(unsigned int line) { //TODO

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