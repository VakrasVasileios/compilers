// #include "../include/Scope.h"
// #include <iostream>

// #define global_scope 0

// static unsigned int    current_scope = 0;
// static unsigned int    anonymous_func_num = 0;
// static unsigned int    loopDepth = 0;

// std::list<std::pair<std::string, unsigned int> >    argList;

// std::list<std::pair<std::string, Variable> >        lvaluesList;

// SymbolTable  symTable;
// ScopeStack   scopeStack;

// std::list<std::pair<std::string, unsigned int> >&
// GetArgList(void) {
//     return argList;
// }

// SymbolTable&
// GetSymbolTable(void) {
//     return symTable;
// }

// void
// Init(void) {
//     symTable.push_back(BlockList());
//     symTable[current_scope].push_back(Block());
//     scopeStack.Push(symTable[current_scope].GetCurrentBlock());

//     scopeStack.Top()->ids.insert({ "print", Variable(LIB_FUNC, true, 1, global_scope) });
//     scopeStack.Top()->ids.insert({ "input", Variable(LIB_FUNC, true, 1, global_scope) });
//     scopeStack.Top()->ids.insert({ "objectmemberkeys", Variable(LIB_FUNC, true, 1, global_scope) });
//     scopeStack.Top()->ids.insert({ "objecttotalmembers", Variable(LIB_FUNC, true, 1, global_scope) });
//     scopeStack.Top()->ids.insert({ "objectcopy", Variable(LIB_FUNC, true, 1, global_scope) });
//     scopeStack.Top()->ids.insert({ "totalarguments", Variable(LIB_FUNC, true, 1, global_scope) });
//     scopeStack.Top()->ids.insert({ "argument", Variable(LIB_FUNC, true, 1, global_scope) });
//     scopeStack.Top()->ids.insert({ "typeof", Variable(LIB_FUNC, true, 1, global_scope) });
//     scopeStack.Top()->ids.insert({ "strtonum", Variable(LIB_FUNC, true, 1, global_scope) });
//     scopeStack.Top()->ids.insert({ "sqrt", Variable(LIB_FUNC, true, 1, global_scope) });
//     scopeStack.Top()->ids.insert({ "cos", Variable(LIB_FUNC, true, 1, global_scope) });
//     scopeStack.Top()->ids.insert({ "sin", Variable(LIB_FUNC, true, 1, global_scope) });
// }

// void
// IncreaseScope(void) {
//     current_scope++;
//     if (current_scope > symTable.size()-1)
//         symTable.push_back(BlockList());
//     symTable[current_scope].push_back(Block());
//     scopeStack.Push(symTable[current_scope].GetCurrentBlock());
// }

// void
// DecreaseScope(void) {
//     scopeStack.Top()->SetVisibilityAll(false);
//     scopeStack.Pop();
//     current_scope--;
// }


// std::pair<const std::string, Variable>*
// Lookup(const char* id, type_t type) {
//     for (auto block : scopeStack) {
//         auto ret = block->ids.find(id);
//         if (ret != block->ids.end() && ((*ret).second.isVisible && (*ret).second.type == type)) {
//             return &*ret;
//         }
//     }
//     return nullptr;
// }

// /*
// std::pair<const std::string, Variable>*
// Lookup(const char* id, type_t type) {
//     if (type != LIB_FUNC || type != GLOBAL_VAR) {  //if id is a user function or local variable
//         auto iter = scopeStack.end();
//         for (; iter != scopeStack.begin(); --iter) {
//             auto ret = (*iter)->ids.find(id);
//             if (ret != (*iter)->ids.end() && ((*ret).second.isVisible && (*ret).second.type == type)) {
//                 return &*ret;
//             }
//         }
//     }
//     else {                                         //if id is a library function or global variable
//         auto ret = (*scopeStack.begin())->ids.find(id);
//         if (ret != (*scopeStack.begin())->ids.end() && ((*ret).second.isVisible && (*ret).second.type == type)) {
//             return &*ret;
//         }
//     }

//     return nullptr;
// }*/

// void
// AddVariable(const char* id, unsigned int line, type_t type) {
//     std::pair<const std::string, Variable>* v = nullptr;
//     if ((v = Lookup(id, LIB_FUNC)) != nullptr || (v = Lookup(id, USER_FUNC)) != nullptr) {
//         std::cout << "Error " << '\"'<< id <<'\"' << " already in use as a function, in line: " << v->second.line << std::endl;
//         return;
//     }
//     else if ((v = Lookup(id, type)) == nullptr) {
//         v = new std::pair<const std::string, Variable>(std::string(id), Variable(type, true, line, current_scope));
//         scopeStack.Top()->ids.insert(*v);
//     }
//     else {
//         scopeStack.Top()->ids.insert(*v);
//     }
// }

// void
// AddFunction(const char* id, unsigned int line) {
//     std::pair<const std::string, Variable>* v = nullptr;
//     if (std::string(id).rfind("$") != std::string::npos) { // check if function about to be added is anonymous
//         v = new std::pair<const std::string, Variable>(std::string(id), Variable(USER_FUNC, true, line, current_scope));
//         scopeStack.Top()->ids.insert(*v);
//         return;
//     }
//     if ((v = Lookup(id, GLOBAL_VAR)) != nullptr || (v = Lookup(id, LOCAL_VAR)) != nullptr) {
//         std::cout << "Error " << '\"'<< id <<'\"' << " already in use as a variable, in line: " << v->second.line << std::endl;
//         return;
//     }
//     else if ((v = Lookup(id, LIB_FUNC)) != nullptr) {
//         std::cout << "Error " << '\"'<< id <<'\"' << " already in use as a library function, in line: " << v->second.line << std::endl;
//         return;
//     }
//     else {
//         v = new std::pair<const std::string, Variable>(std::string(id), Variable(USER_FUNC, true, line, current_scope));
//         scopeStack.Top()->ids.insert(*v);
//         return;
//     }
// }

// void
// AddAnonymousFunction(unsigned int line) {
//     std::string _id = "$";
//     _id += std::to_string(anonymous_func_num);
//     AddFunction(_id.c_str(), line);
//     ++anonymous_func_num;
// }

// void
// HideLowerScopes(void) {
//     auto iter = scopeStack.begin();
//     for (; iter != scopeStack.end(); ++iter) {
//         (*iter)->SetVisibilityAll(false);
//     }
// }

// void
// HideLowerScopesExceptArgs(void) {
//     HideLowerScopes();
//     auto iter = argList.begin();
//     for (auto block : scopeStack) {
//         block->SetIDVisibility((*iter).first, true);
//         ++iter;
//     }
// }

// void
// EnableLowerScopes(void) {
//     auto iter = scopeStack.begin();
//     for (; iter != scopeStack.end(); ++iter) {
//         (*iter)->SetVisibilityAll(true);
//     }
// }

// void
// CleanArgs(void) {
//     argList.clear();
// }

// void
// EnableLowerScopesAndCleanArgs(void) {
//     EnableLowerScopes();
//     CleanArgs();
// }

// void
// AddFormalArgs(void) {
//     for (auto iter = argList.begin(); iter != argList.end(); iter++) {
//         scopeStack.Top()->ids.insert({ (*iter).first, Variable(FORMAL_ARG, true, (*iter).second, current_scope) });
//     }
//     CleanArgs();
// }

// void
// StashLvalue(const char* id, unsigned int line) {
//     if (Lookup(id, USER_FUNC) == nullptr && Lookup(id, LIB_FUNC) == nullptr) {
//         type_t type = current_scope == 0 ? GLOBAL_VAR : LOCAL_VAR;
//         std::pair<const std::string, Variable> v = std::pair<const std::string, Variable>(std::string(id), Variable(type, true, line, current_scope));
//         lvaluesList.push_back(v);
//     }
// }

// void
// AddStashedLvalues(void) {
//     for (auto iter : lvaluesList) {
//         AddVariable(iter.first.c_str(), iter.second.line, iter.second.type);
//     }
//     CleanLvaluesStash();
// }

// void
// CleanLvaluesStash(void) {
//     lvaluesList.clear();
// }

// unsigned int
// GetScope(void) { return current_scope; }

// unsigned int
// GetLoopDepth(void) { return loopDepth; }

// void
// IncreaseLoopDepth(void) { ++loopDepth; }

// void
// DecreaseLoopDepth(void) { --loopDepth; }