#include <list>
#include <utility>
#include "ScopeStack.h"
#include "SymbolTable.h"

#ifndef SCOPE
#define SCOPE

void    Init(void);
void    IncreaseScope(void);
void    DecreaseScope(void);
auto    Lookup(const char* id) -> std::pair<const std::string, Variable>*;
auto    LookupUpToGlobal(const char* id) -> std::pair<const std::string, Variable>*;
void    AddVariable(const char* id, unsigned int line);
void    AddFunction(const char* id, unsigned int line);
void    AddAnonymousFunction(unsigned int line);
void    HideLowerScopes(void);
void    HideLowerScopesExceptArgs(void);
void    EnableLowerScopes(void);
void    EnableLowerScopesAndCleanArgs(void);
void    AddFormalArgs(void);
void    CleanArgs(void);

std::list<std::pair<std::string, unsigned int> >& GetArgList(void);
SymbolTable&            GetSymbolTable(void);

#endif