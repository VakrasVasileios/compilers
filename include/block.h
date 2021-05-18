#ifndef BLOCK_H
#define BLOCK_H

#include "expression/symbol.h"
#include "expression/function.h"
#include "../util/contract/contract.h"
#include <list>

/**
 * @brief Contains multiple symbols across a single scope.
 * 
**/ 
class Block final {
    public:
        Block() = default;
        ~Block() = default;
        /**
         * @brief Returns a read access to this Block symbols.
         * 
         * @return a read access to this Block symbols, not null
        **/ 
        std::list<Symbol*>      get_symbols() const;
        /**
         * @brief Activates all of the symbols of this Block..
         * 
        **/ 
        void                    Activate();
        /**
         * @brief Deactivates all of the symbols of this Block..
         * 
        **/ 
        void                    Deactivate();   
        /**
         * @brief Searches for a symbol with an id on this Block.
         * Returns a read/write access to a reference of the symbol if it's found,
        *  else nullptr.
        * 
        *  @param id the id with which to search for a symbol on this Block
        * 
        *  @return a read/write access to a reference of the symbol if it's found,
        *  else nullptr
        **/ 
        Symbol*                 Lookup(std::string id);
        /**
         * @brief Inserts a symbol at the end of this Block.
         * 
         * @param symbol the symbol to be inserted at the end
         * of this Block, not null
         * 
        **/ 
        void                    Insert(Symbol* symbol);
        /**
         * @brief Checks wether this Block contains only hidden variables.
         * 
         * @return wether this Block contains only hidden variables
        **/ 
        bool                    ContainsOnlyHiddenVars();

        friend std::ostream&    operator<<(std::ostream& os, const Block block);
    private:
       std::list<Symbol*> entries;

       static bool                     IsHiddenVariable(Symbol* entry);
};

#endif