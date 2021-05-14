#ifndef PROGRAMSTACK_H
#define PROGRAMSTACK_H

#include "block.h"
#include "expression/symbol/function/function.h"
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
    /*
    Returns a read access to this ProgramStack blocks.
    */
    std::list<Block*>   get_block_list() const;
    /*
    Returns a read/write access to this ProgramStack top block.
    */
    Block*              Top();
    /*
    Pushes a block at the top of this ProgramStack.
    */
    void                Push(Block* block);
    /*
    Pops a block from the top of this ProgramStack.
    */
    void                Pop();
    /*
    Searches for a symbol with a name on this ProgramStack,
    across all scopes. Returns a read/write access to the symbol if it's found,
    else nullptr.
    */
    Symbol*             Lookup(std::string name);
    /*
    Searches for a symbol with a name on this ProgramStack,
    across the global scope. Returns a read/write access to the symbol if it's found,
    else nullptr.
    */
    Symbol*             LookupGlobal(std::string name);
    /*
    Searches for a hidden variable with a name at the current scope of this ProgramStack.
    Returns a read/write access to the reference of the hidden variable if it's found,
    else nullptr.
    */
    Symbol*             LookupHiddenVariable(std::string name);
    /*
    Activates all of the blocks at the lower scopes of this ProgramStack.
    */
    void                ActivateLowerScopes();
    /*
    Deactivates all of the blocks variables at the lower scopes of this ProgramStack.
    */
    void                DeactivateLowerScopes();
private:
    std::list<Block*>   block_list;  
};

#endif