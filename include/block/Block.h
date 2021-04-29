#ifndef BLOCK_H
#define BLOCK_H

#include "../symbol_table/symbol_table_entry/SymbolTableEntry.h"
#include <list>
#include <map>

/*
Provides a block containing symbol table entries.
*/
class Block final {
    private:
        std::list<SymbolTableEntry> symbolTableEntries;
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
        Returns the list of all symbol table entries of this Block.
        */
        std::list<SymbolTableEntry>     getSymbolTableEntries() const;  
        /*
        Adds a symbol table entry to this Block.
        */
        void                            addSymbolTableEntry(SymbolTableEntry entry);

        void                            hide();

        friend std::ostream&    operator<<(std::ostream& os, const SymbolTable symbolTable);
};

// struct Block {
//     std::map<const std::string, SymbolTableEntry>  entries;
//     Block() = default;
//     Block(std::map<const std::string, SymbolTableEntry> entries) : entries(entries) {};

//     void    setSymbolTableEntryVisibility(const std::string name, bool visible);
//     void    setVisibilityAll(bool visible);

//     void    logBlock(void);
// };

#endif