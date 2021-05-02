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
        /*
        Returns a read access to the list of block references of this ProgramStack.
        */
        std::list<Block*>  get_block_list() const;
        /*
        Returns read/write access to the top block reference of this ProgramSTack.
        */
        Block*              Top();
        /*
        Pushes a block reference on this ProgramStack.
        */
        void                Push(Block* block);
        /*
        Pops a block reference from this ProgramStack.
        */
        void                Pop();
        /*
        Activates all of the symbol table entries at lower scopes, except the global scope.
        */
        void                ActivateLowerScopes();
        /*
        Deactivates all of the symbol table entries at lower scopes, except the global scope.
        */
        void                DeactivateLowerScopes();
        /*
        Searches for an active symbol table entry by its name across all scopes. 
        Returns a read/write access to the reference of the active entry if it's found, else nullptr.
        */
        SymbolTableEntry*   Lookup(std::string name);
        /*
        Searches for an active symbol table entry by its name across the global scope. 
        Returns a read/write access to the reference of the active entry if it's found, else nullptr.
        */
        SymbolTableEntry*   LookupGlobal(std::string name);
    private:
        std::list<Block*>  block_list;
};

#endif