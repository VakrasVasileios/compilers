#include <list>
#include <utility>

#ifndef SCOPE
#define SCOPE

#include "ScopeStack.h"
#include "SymbolTable.h"

void    init(void);

void    increaseScope(void);
void    decreaseScope(void);

auto    lookup(const char* id, SymbolType type);

void    addVariable(const char* id, unsigned int line, SymbolType type);
void    addFunction(const char* id, unsigned int line);
void    addAnonymousFunction(unsigned int line);

void    hideLowerScopes(void);
void    hideLowerScopesExceptArgs(void);
void    enableLowerScopes(void);
void    enableLowerScopesAndCleanArgs(void);

void    addFormalArgs(void);

void    stashLvalue(const char* id, unsigned int line);
void    addStashedLvalues(void);
void    cleanLvaluesStash(void);

std::list<std::pair<std::string, unsigned int> >& getArgList(void);
std::list<std::pair<std::string, unsigned int> >& getLvaluesList(void);

SymbolTable&    getSymbolTable(void);

unsigned int    getScope(void);
unsigned int    getLoopDepth(void);
void            increaseLoopDepth(void);
void            decreaseLoopDepth(void);

#endif