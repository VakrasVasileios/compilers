#ifndef BLOCK_STACK_H
#define BLOCK_STACK_H

#include <list>
#include "../block/Block.h"

/*
Provides a pseudo-stack containing all the blocks.
*/
class BlockStack {
    private:
        std::list<Block*> blockStack;
    public:
        /* 
        The Default Constructor of this BlockStack.
        */
        BlockStack() = default;
        /*
        The Default Destructor of this BlockStack.
        */
        ~BlockStack() = default;
        /*
        Returns the top, active block, at the current scope, of this BlockStack.
        */
        Block*              topBlock(void);
        /*
        Pushes a block at the top of this BlockStack.
        */
        void                pushBlock(Block* block);
        /*
        Hides and pops the block at the top of this BlockStack.
        */
        void                popBlock(void);
        /*
        Searches for a symbol table entry across all blocks of this BlockStack.
        */
        SymbolTableEntry*   Lookup(const char* name);
        /*
        Hides all the entries, of the blocks of this BlockStack, at the lower scopes down to the global scope.
        */
        void                hideLowerScopes();
        /*
        Enables all the entries, of the blocks of this BlockStack, at the lower scopes down to the global scope.
        */
        void                enableLowerScopes();
};

#endif
