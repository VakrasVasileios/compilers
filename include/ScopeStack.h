#include <list>
#include "Block.h"

#ifndef SCOPE_STACK
#define SCOPE_STACK

class ScopeStack : public std::list<Block*> {
public:
    ScopeStack() = default;
    ~ScopeStack() = default;

    Block*  Top(void)           { return !empty() ? *(--end()) : nullptr; }
    void    Push(Block* block)  { push_back(block); }
    void    Pop(void)           { pop_back(); }
};

#endif
