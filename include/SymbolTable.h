#include <vector>
#include <list>
#include "BlockList.h"

#ifndef SYMBOL_TABLE
#define SYMBOL_TABLE

class SymbolTable : public std::vector<BlockList> {
public:
    void    DeactivateLowerScopes(int curr_scope);
    void    ReActivateLowerScopes(int curr_scope);
};

#endif