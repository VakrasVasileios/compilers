#include "../include/SymbolTable.h"
#include <iostream>

void
SymbolTable::logSymbolTable(void) {
    for (int i = 0; i < (*this).size(); i++){
        std::cout << std::endl;
        std::cout << "-----------    Scope #" << i << "    -----------" << std::endl;
        (*this)[i].logBlockList();
    }
}
