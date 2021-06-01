#ifndef PROGRAMSTACK_H
#define PROGRAMSTACK_H

#include <list>
#include <string>
#include "../../../../util/contract/contract.h"
#include "block.h"

namespace syntax_analysis {
    /**
     * @brief A stack containing all of the scope blocks. 
     * The top block is always the current scope block.
     * 
    **/ 
    class ProgramStack final {
    public:
        ProgramStack() = default;
        ~ProgramStack() = default;
        /**
         * @brief Returns a read access to this ProgramStack blocks.
         * 
         * @return  a read access to this ProgramStack blocks, not null
         * 
        **/ 
        std::list<Block*>   get_block_list() const;
        /**
         * @brief Returns a read/write access to this ProgramStack top block.
         * 
         * @return a read/write access to this ProgramStack top block, can be null.
         * 
        **/ 
        Block*              Top();
        /**
         * @brief Pushes a block at the top of this ProgramStack.
         * 
         * @param block the block to be pushed at the top of this
         * ProgramStack, not null
         * 
        **/ 
        void                Push(Block* block);
        /**
         * @brief Pops a block from the top of this ProgramStack.
         * 
        **/ 
        void                Pop();
        /**
         * @brief Searches for a symbol with a name on this ProgramStack,
         * across all scopes. Returns a read/write access to the symbol if it's found,
         * else nullptr.
         * 
         * @return a read/write access to the symbol if it's found,
         * else nullptr.
         * 
        **/ 
        expression::Symbol* Lookup(std::string name);
        /**
         * @brief Searches for a symbol with a name on this ProgramStack,
         * across the global scope. Returns a read/write access to the symbol if it's found,
         * else nullptr.
         * 
         * @return a read/write access to the symbol if it's found,
         * else nullptr.
         * 
        **/ 
        expression::Symbol* LookupGlobal(std::string name);
        /**
         * @brief Activates all of the blocks at the lower scopes of this ProgramStack.
         * 
        **/ 
        void                ActivateLowerScopes();
        /**
         * @brief Deactivates all of the blocks variables at the lower scopes of this ProgramStack.
         * 
        **/  
        void                DeactivateLowerScopes();
    private:
        std::list<Block*>   block_list;  
    };
}

#endif