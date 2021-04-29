#include "../../include/parser_manager/ParserManager.h"
#include "../../include/symbol_table/SymbolTable.h"
#include "../../include/symbol_table/symbol_table_entry/SymbolTableEntry.h"
#include "../../include/symbol_table/symbol_table_entry/variable_entry/FormalVariableEntry.h"
#include <string>
#include <list>
#include <iostream>

#define LIB_FUNC_LINE -1
#define OUT_OF_SCOPE  -1

const unsigned int global_scope = 0;
unsigned int current_scope = OUT_OF_SCOPE;

bool methodCall = false;
bool validReturn = false;

unsigned int loopDepth = 0;

unsigned int anonymusFuncsCounter = 0;

SymbolTable symbolTable;

std::list<Block*>  blockList;

std::list<FormalVariableEntry> stashedFormalArguments; //End element always holds a reference to the new block thats pushed to the symbol table.

void init_library_functions() {
    increase_scope();
    (*--blockList.end())->addSymbolTableEntry(LibraryFunctionEntry("print", LIB_FUNC_LINE, global_scope));
    (*--blockList.end())->addSymbolTableEntry(LibraryFunctionEntry("input", LIB_FUNC_LINE, global_scope));
    (*--blockList.end())->addSymbolTableEntry(LibraryFunctionEntry("objectmemberkeys", LIB_FUNC_LINE, global_scope));
    (*--blockList.end())->addSymbolTableEntry(LibraryFunctionEntry("objecttotalmembers", LIB_FUNC_LINE, global_scope));
    (*--blockList.end())->addSymbolTableEntry(LibraryFunctionEntry("objectcopy", LIB_FUNC_LINE, global_scope));
    (*--blockList.end())->addSymbolTableEntry(LibraryFunctionEntry("totalarguments", LIB_FUNC_LINE, global_scope));
    (*--blockList.end())->addSymbolTableEntry(LibraryFunctionEntry("argument", LIB_FUNC_LINE, global_scope));
    (*--blockList.end())->addSymbolTableEntry(LibraryFunctionEntry("typeof", LIB_FUNC_LINE, global_scope));
    (*--blockList.end())->addSymbolTableEntry(LibraryFunctionEntry("strtonum", LIB_FUNC_LINE, global_scope));
    (*--blockList.end())->addSymbolTableEntry(LibraryFunctionEntry("sqrt", LIB_FUNC_LINE, global_scope));
    (*--blockList.end())->addSymbolTableEntry(LibraryFunctionEntry("cos", LIB_FUNC_LINE, global_scope));
    (*--blockList.end())->addSymbolTableEntry(LibraryFunctionEntry("sin", LIB_FUNC_LINE, global_scope));
}

void increase_scope() {
    Block newBlock;
    symbolTable.insert(++current_scope, newBlock);
    blockList.push_back(&newBlock);
}

void decrease_scope() {
    blockList.pop_back();
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
    auto iter = blockList.begin();
    for (; iter != blockList.end(); ++iter) {
        for (auto entry : (*iter)->getSymbolTableEntries())
        {
            entry.setActive(false);
        }
    }
}

void enable_lower_scopes() {
    auto iter = blockList.begin();
    for (; iter != blockList.end(); ++iter) {
        for (auto entry : (*iter)->getSymbolTableEntries())
        {
            entry.setActive(true);
        }
    }
}

// SymbolTableEntry*   LookupGlobal(const char* name) { ????
//     auto iter = blockStack.begin();
//     auto b = (*iter)->getSymbolTableEntries();
//     for (auto i : b) {
//         if (i.getId().c_str() == name)
//             return &i; 
//     }

//     return nullptr;
// }

SymbolTableEntry* lookup(const char* name) {
    auto iter = blockList.end();
    do {
        --iter;
        auto b = (*iter)->getSymbolTableEntries();
        for (auto i : b) {
            if (i.getId().c_str() == name)
                return &i; 
        }
    } while (iter != blockList.begin());

    return nullptr;
}

bool lookup_library_function(const char* name) {
    SymbolTableEntry *r_value = lookup(name);
    if(r_value == nullptr || r_value->getType() != LIB_FUNC)
        return false;
    else
        return true;
}

bool lookup_user_function(const char* name) {
    SymbolTableEntry *r_value = lookup(name);
    if(r_value == nullptr || r_value->getType() != USER_FUNC)
        return false;
    else
        return true;
}

bool lookup_local_variable(const char* name) {
    SymbolTableEntry *r_value = lookup(name);
    if(r_value == nullptr || r_value->getType() != LOCAL_VAR)
        return false;
    else
        return true;
}

bool lookup_global_variable(const char* name) {
    SymbolTableEntry *r_value = lookup(name);
    if(r_value == nullptr)
        return false;
    else
        return true;
}

bool lookup_formal_variable(const char* name) {
    SymbolTableEntry *r_value = lookup(name);
    if(r_value == nullptr || r_value->getType() != FORMAL_VAR)
        return false;
    else
        return true;
}

void insert_user_function(const char* name, unsigned int line) {
    blockList.top()->addSymbolTableEntry(UserFunctionEntry(name, line, current_scope, stashedFormalArguments)); /////??????????????????????????????? TA BAZOYME 2 FORES.
}

void insert_user_function(unsigned int line) {
    std::string an = "$";
    an += anonymusFuncsCounter;
    blockList.top()->addSymbolTableEntry(UserFunctionEntry(an, line, current_scope, stashedFormalArguments)); /////??????????????????????????????? TA BAZOYME 2 FORES.
}

void push_stashed_lvalues() { //TODO?

}

void stash_lvalue(const char* name, unsigned int line) { //TODO?

}

void reset_lvalues_stash() { //TODO?

}

void push_stashed_formal_arguments(void) { /////??????????????????????????????? TA BAZOYME 2 FORES.
    for (auto i : stashedFormalArguments) {
        blockList.top()->addSymbolTableEntry(i);
    }
    stashedFormalArguments.clear();
}

void stash_formal_argument(const char* name, unsigned int line) {
    stashedFormalArguments.push_back(FormalVariableEntry(name, line, current_scope + 1));
}

void log_symbol_table() {
    std::cout << symbolTable;
}