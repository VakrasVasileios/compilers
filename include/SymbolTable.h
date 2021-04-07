#include <vector>
#include <list>
#include "BlockList.h"

#ifndef SYMBOL_TABLE
#define SYMBOL_TABLE

class SymbolTable : public std::vector<BlockList> {
public:
    SymbolTable() = default;
    ~SymbolTable() = default;

    void        logSymbolTable(void);
};

#endif