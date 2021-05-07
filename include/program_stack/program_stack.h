#ifndef PROGRAMSTACK_H
#define PROGRAMSTACK_H

#include "../block/block.h"

#include <list>
#include <string>

/*
Provides a stack containing all the scope blocks. 
The top block is always the current scope block.
*/
class ProgramStack final {
public:
    ProgramStack() = default;
    ~ProgramStack() = default;

    std::list<Block*>   get_block_list() const;

    Block*              Top();
    void                Push(Block* block);
    void                Pop();

    Symbol*             Lookup(std::string name);
    Symbol*             LookupGlobal(std::string name);

    Symbol*             LookupFunc(std::string name);

    void                ActivateLowerScopes();
    void                DeactivateLowerScopes();
private:
    std::list<Block*>   block_list;  
};

#endif