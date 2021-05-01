#ifndef PROGRAMSTACK_H
#define PROGRAMSTACK_H

#include "../block/Block.h"
#include <list>
#include <string>

/*
Provides a stack containing all the scope blocks. 
The top block is always the current scope block.
*/
class ProgramStack {
    private:
        std::list<Block*>  blockList;
    public:
        /*
        The Default Constructor of this ProgramStack.
        */
        ProgramStack() = default;
        /*
        The Default Destructor of this ProgramStack.
        */
        ~ProgramStack() = default;
        /*
        Returns the blockList of this ProgramStack.
        */
        std::list<Block*>  getBlockList() const;
        /*
        Returns read/write access to the top block of this ProgramSTack.
        */
        Block*              top();
        /*
        Pushes a block on this ProgramStack.
        */
        void                push(Block* block);
        /*
        Pops a block from this ProgramStack.
        */
        void                pop();
        /*
        Activates all of the symbol table entries at lower scopes, except the global scope.
        */
        void                activateLowerScopes();
        /*
        Deactivates all of the symbol table entries at lower scopes, except the global scope.
        */
        void                deactivateLowerScopes();
        /*
        Searches for an active symbol table entry by its name across all scopes. 
        Returns a reference of the entry if it's found and it's active, else nullptr.
        */
        SymbolTableEntry*   lookup(std::string name);
        /*
        Searches for an active symbol table entry by its name across the global scope. 
        Returns a reference of the entry if it's found and it's active, else nullptr.
        */
        SymbolTableEntry*   lookupGlobal(std::string name);
};

#endif