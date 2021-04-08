#include <list>
#include <utility>

#ifndef SCOPE
#define SCOPE

#include "ScopeStack.h"
#include "SymbolTable.h"

void    Init(void);

void    IncreaseScope(void);
void    DecreaseScope(void);

auto    Lookup(const char* id, type_t type) -> std::pair<const std::string, Variable>*;

void    AddVariable(const char* id, unsigned int line, type_t type);
void    AddFunction(const char* id, unsigned int line);
void    AddAnonymousFunction(unsigned int line);

void    HideLowerScopes(void);
void    HideLowerScopesExceptArgs(void);
void    EnableLowerScopes(void);
void    EnableLowerScopesAndCleanArgs(void);

void    AddFormalArgs(void);

void    StashLvalue(const char* id, unsigned int line, type_t type);
void    AddStashedLvalues(void);

std::list<std::pair<std::string, unsigned int> >& GetArgList(void);
std::list<std::pair<std::string, unsigned int> >& GetLvaluesList(void);

SymbolTable&    GetSymbolTable(void);

unsigned int    GetScope(void);
unsigned int    GetLoopDepth(void);
void            IncreaseLoopDepth(void);
void            DecreaseLoopDepth(void);

#endif