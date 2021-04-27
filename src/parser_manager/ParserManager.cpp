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

void increase_scope() {
    current_scope++;
}

void decrease_scope() {
    current_scope--;
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
