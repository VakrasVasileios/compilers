#ifndef BLOCK_H
#define BLOCK_H

#include "../symbol_table/symbol_table_entry/SymbolTableEntry.h"
#include <list>

/*
Provides a block containing ids across a single scope.
*/
class Block {
    private:
       std::list<SymbolTableEntry*> entries;
    public:
        /*
        The Default Constructor of this Block.
        */
        Block() = default;
        /*
        The Default Destructor of this Block.
        */
        ~Block() = default;
        /*
        Activates all the entries on this Block.
        */
        void                    activate();
        /*
        Deactivates all the entries on this Block.
        */
        void                    deactivate();   
        /*
        Searches for an active symbol table entry with an id on this Block.
        Returns a reference to the active entry if its found, else nullptr.
        */
        SymbolTableEntry*       lookup(std::string id);
        /*
        Inserts a symbol table entry to the end of the Block.
        */
        void                    insert(SymbolTableEntry* entry);

        friend std::ostream&    operator<<(std::ostream& os, const Block block);
};

#endif