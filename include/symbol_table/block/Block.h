#include "symbol_table_entry/SymbolTableEntry.h"
#include <vector>

#ifndef BLOCK
#define BLOCK

/*
Provides a block containing symbol table entries.
*/
class Block final {
    private:
        std::vector<SymbolTableEntry> symbolTableEntries;
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
        std::vector<SymbolTableEntry>   getSymbolTableEntries() const;  
        /*
        Adds a symbol table entry to this Block.
        */
        void                            addSymbolTableEntry(SymbolTableEntry entry);
};

// struct Block {
//     std::map<const std::string, Variable>  ids;
//     Block() = default;
//     Block(std::map<const std::string, Variable> _ids) : ids(_ids) {};

//     void    SetIDVisibility(const std::string& _id, bool _visible);
//     void    SetVisibilityAll(bool _visible);

//     void    logBlock(void);
// };

#endif