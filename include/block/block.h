#ifndef BLOCK_H
#define BLOCK_H

#include "../expression/symbol/symbol.h"
#include <list>

class Block final {
    public:
        Block() = default;
        ~Block() = default;

        std::list<Symbol*>      get_entries() const;

        void                    Activate();
        /*
        Deactivates all the variables of this Block.
        */
        void                    Deactivate();   

        Symbol*                 Lookup(std::string id);
        Symbol*                 LookupFunc(std::string id);

        void                    Insert(Symbol* symbol);

        friend std::ostream&    operator<<(std::ostream& os, const Block block);
    private:
       std::list<Symbol*> entries;
};

#endif