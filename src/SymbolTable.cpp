#include "../include/SymbolTable.h"

void
SymbolTable::DeactivateLowerScopes(int curr_scope) {
    auto iter = begin();
    int i = 1;
    for (; i < curr_scope; i++) {
        ++iter;
        (*iter).DeactivateBlock();
    }
}

void
SymbolTable::ReActivateLowerScopes(int curr_scope) {
    auto iter = begin();
    int i = 1;
    for (; i < curr_scope; i++) {
        ++iter;
        (*iter).ReActivateBlock();
    }
}