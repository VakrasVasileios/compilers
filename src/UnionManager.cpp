#include "../include/UnionManager.h"
#include <iostream>
#include <algorithm>

UnionManager::UnionManager() {
    InitLibraryFuncs();
}

void
UnionManager::RegisterAnonimousFunc(void) {
    RegisterFunc(std::string("$"+anonymous_func_num++).c_str());
}

void 
UnionManager::RegisterVar(const char* id) {
    if(IsFuncInLibraryTableOrActiveBlock(id) || IsVarInActiveBlock(id)) 
        return;

    Block b = *(variableTable[current_scope].GetActiveBlock());
    b.ids.push_back(std::string(id));
}

void
UnionManager::RegisterFunc(const char* id) {
    if(IsFuncInLibraryTableOrActiveBlock(id)) 
        return;
    functionTable[current_scope].push_back(Block({std::string(id)}, current_scope, true));
}

bool
UnionManager::IsVarInActiveBlock(const char* id) {
    for (unsigned int  i = global_scope; i < current_scope; i++) {
        Block* block = variableTable[i].GetActiveBlock();
        if (block != nullptr) {
            auto it = std::find((*block).ids.begin(), (*block).ids.end(), std::string(id));
            if (it != block->ids.end()) {
                std::cout << "Error, there is a variable with the same name already declared" << std::endl;
                return true;
            }
        }
    }
    return false;
}

bool
UnionManager:: IsFuncInLibraryTableOrActiveBlock(const char* id) {
    return IsFuncInLibraryTable(id) || IsFuncInActiveBlock(id);
}

bool
UnionManager:: IsFuncInLibraryTable(const char* id) {
    std::string f = std::string(id);
    std::cout << "Error, there is a library function with the same name already declared" << std::endl;
    return libraryFuncTable.find(f) != libraryFuncTable.end();
}

bool
UnionManager::IsFuncInActiveBlock(const char* id) {
    for (unsigned int  i = global_scope; i < current_scope; i++) {
        Block* block = functionTable[i].GetActiveBlock();
        if(block != nullptr) {
            auto i = std::find((*block).ids.begin(), (*block).ids.end(), std::string(id));
            if (i != block->ids.end()) {
                std::cout << "Error, there is a function with the same name already declared" << std::endl;
                return true;
            }
        }
    }
    return false;
}

void
UnionManager::InitLibraryFuncs(void) {
    libraryFuncTable = {
        {"print", true}, {"input", true}, {"objectmemberkeys", true}, {"objecttotalmembers", true},
        {"objectcopy", true}, {"totalarguments", true}, {"argument", true}, {"typeof", true},
        {"strtonum", true}, {"sqrt", true}, {"cos", true}, {"sin", true}
    };
}