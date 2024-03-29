#ifndef BLOCK_H
#define BLOCK_H

#include <list>
#include <iostream>
#include "../../../../../util/contract/include/contract.h"
#include "../../expression/include/expression.h"

namespace target_code
{
    namespace syntax_analysis 
    {
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
        std::list<expression::Symbol*>  get_symbols() const;
        /**
         * @brief Activates all of the symbols of this Block..
         * 
        **/ 
        void                            Activate();
        /**
         * @brief Deactivates all of the symbols of this Block..
         * 
        **/ 
        void                            Deactivate();   
        /**
         * @brief Searches for a symbol with an id on this Block.
         * Returns a read/write access to a reference of the symbol
         * if it's found, else nullptr.
        * 
        *  @param id the id with which to search for a symbol on this Block
        * 
        *  @return a read/write access to a reference of the symbol if it's
        *  found, else nullptr
        **/ 
        expression::Symbol*             Lookup(std::string id);
        /**
         * @brief Inserts a symbol at the end of this Block.
         * 
         * @param symbol the symbol to be inserted at the end
         * of this Block, not null
         * 
        **/ 
        void                            Insert(expression::Symbol* symbol);

        friend std::ostream&            operator<<(std::ostream& os,
                                            const Block block);
    private:
        std::list<expression::Symbol*>  symbols;   
    };
    }
}

#endif