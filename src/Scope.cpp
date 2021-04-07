#include "../include/Scope.h"
#include <iostream>

#define global_scope 0

unsigned int current_scope = 0;
unsigned int anonymous_func_num = 0;

std::list<std::pair<std::string, unsigned int> > argList;

SymbolTable  symTable;
ScopeStack   scopeStack;

std::list<std::pair<std::string, unsigned int> >&
GetArgList(void) {
    return argList;
}

SymbolTable&
GetSymbolTable(void) {
    return symTable;
}

void
Init(void) {
    symTable.push_back(BlockList());
    symTable[current_scope].push_back(Block());
    scopeStack.Push(symTable[current_scope].GetCurrentBlock());

    scopeStack.Top()->ids.insert({ "print", Variable(LIB_FUNC, true, 1, global_scope) });
    scopeStack.Top()->ids.insert({ "input", Variable(LIB_FUNC, true, 1, global_scope) });
    scopeStack.Top()->ids.insert({ "objectmemberkeys", Variable(LIB_FUNC, true, 1, global_scope) });
    scopeStack.Top()->ids.insert({ "objecttotalmembers", Variable(LIB_FUNC, true, 1, global_scope) });
    scopeStack.Top()->ids.insert({ "objectcopy", Variable(LIB_FUNC, true, 1, global_scope) });
    scopeStack.Top()->ids.insert({ "totalarguments", Variable(LIB_FUNC, true, 1, global_scope) });
    scopeStack.Top()->ids.insert({ "argument", Variable(LIB_FUNC, true, 1, global_scope) });
    scopeStack.Top()->ids.insert({ "typeof", Variable(LIB_FUNC, true, 1, global_scope) });
    scopeStack.Top()->ids.insert({ "strtonum", Variable(LIB_FUNC, true, 1, global_scope) });
    scopeStack.Top()->ids.insert({ "sqrt", Variable(LIB_FUNC, true, 1, global_scope) });
    scopeStack.Top()->ids.insert({ "cos", Variable(LIB_FUNC, true, 1, global_scope) });
    scopeStack.Top()->ids.insert({ "sin", Variable(LIB_FUNC, true, 1, global_scope) });
}

void
IncreaseScope(void) {
    current_scope++;
    if (current_scope > symTable.size()-1)
        symTable.push_back(BlockList());
    symTable[current_scope].push_back(Block());
    scopeStack.Push(symTable[current_scope].GetCurrentBlock());
}

void
DecreaseScope(void) {
    scopeStack.Top()->SetVisibilityAll(false);
    scopeStack.Pop();
    current_scope--;
}

std::pair<const std::string, Variable>*
Lookup(const char* id) {
    for (auto block : scopeStack) {
        auto ret = block->ids.find(id);
        if (ret != block->ids.end()) {
            return &*ret;
        }
    }
    return nullptr;
}

std::pair<const std::string, Variable>*
LookupUpToGlobal(const char* id) {
    for (auto iter = scopeStack.begin(); iter != scopeStack.end(); ++iter) {
        auto ret = (*iter)->ids.find(id);
        if (ret != (*iter)->ids.end()) {
            return &*ret;
        }
    }
    return nullptr;
}

void
AddVariable(const char* id, unsigned int line) {
    auto v = Lookup(id);
    if (v == nullptr) {
        if (current_scope != global_scope)
            v = new std::pair<const std::string, Variable>(std::string(id), Variable(LOCAL_VAR, true, line, current_scope));
        else
            v = new std::pair<const std::string, Variable>(std::string(id), Variable(GLOBAL_VAR, true, line, current_scope));
        scopeStack.Top()->ids.insert(*v);
    }
    else if (v->second.isVisible && (v->second.type == LIB_FUNC || v->second.type == USER_FUNC)) {
        std::cout << "Error id already in use as a function!" << std::endl;
        return;
    }
    else {
        scopeStack.Top()->ids.insert(*v);
    }
}

void
AddFunction(const char* id, unsigned int line) {
    std::pair<const std::string, Variable>* v = nullptr;
    if (std::string(id).rfind("$") != std::string::npos) // check if function about to be added is anonymous
        v = Lookup(id);
    if (v == nullptr) {
        v = new std::pair<const std::string, Variable>(std::string(id), Variable(USER_FUNC, true, line, current_scope));
        scopeStack.Top()->ids.insert(*v);
        return;
    }
    else if (v->second.isVisible && (v->second.type == GLOBAL_VAR || v->second.type == LOCAL_VAR)) {
        std::cout << "Error id already in use as a variable!" << std::endl;
        return;
    }
    else if (v->second.isVisible && v->second.type == LIB_FUNC) {
        std::cout << "Error id already in use as a library function!" << std::endl;
        return;
    }
    else {
        v = new std::pair<const std::string, Variable>(std::string(id), Variable(USER_FUNC, true, line, current_scope));
        scopeStack.Top()->ids.insert(*v);
        return;
    }
}

void
AddAnonymousFunction(unsigned int line) {
    std::string _id = "$";
    _id += std::to_string(anonymous_func_num);
    AddFunction(_id.c_str(), line);
    ++anonymous_func_num;
}

void
HideLowerScopes(void) {
    auto iter = scopeStack.begin();
    for (; iter != scopeStack.end(); ++iter) {
        (*iter)->SetVisibilityAll(false);
    }
}

void
HideLowerScopesExceptArgs(void) {
    HideLowerScopes();
    auto iter = argList.begin();
    for (auto block : scopeStack) {
        block->SetIDVisibility((*iter).first, true);
        ++iter;
    }
}

void
EnableLowerScopes(void) {
    auto iter = scopeStack.begin();
    for (; iter != scopeStack.end(); ++iter) {
        (*iter)->SetVisibilityAll(true);
    }
}

void
EnableLowerScopesAndCleanArgs(void) {
    EnableLowerScopes();
    CleanArgs();
}

void
AddFormalArgs(void) {
    for (auto iter = argList.begin(); iter != argList.end(); iter++) {
        scopeStack.Top()->ids.insert({ (*iter).first, Variable(FORMAL_ARG, true, (*iter).second, current_scope) });
    }
    CleanArgs();
}

void
CleanArgs(void) {
    argList.clear();
}