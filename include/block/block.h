#ifndef BLOCK_H
#define BLOCK_H

#include "../symbol_table/symbol_table_entry/symbol_table_entry.h"
#include <list>

/*
Provides a block, containing a list of references to
symbol table entries, across a single scope.
*/
class Block final {
    public:
        Block() = default;
        ~Block() = default;
        /*
        Returns a read access to this Block list of references to symbol table entries.
        */
        std::list<SymbolTableEntry*> get_entries() const;
        /*
        Activates all of the symbol table entries on this Block.
        */
        void                        Activate();
        /*
        Deactivates all of the symbol table entries on this Block.
        */
        void                        Deactivate();   
        /*
        Searches for an active symbol table entry with an id on this Block.
        Returns a read/write reference to the active entry if its found, else nullptr.
        */
        SymbolTableEntry*           Lookup(std::string id);
        /*
        Inserts a symbol table entry reference to the end of this Block.
        */
        void                        Insert(SymbolTableEntry* entry);

        friend std::ostream&        operator<<(std::ostream& os, const Block block);
    private:
       std::list<SymbolTableEntry*> entries;
};

#endif