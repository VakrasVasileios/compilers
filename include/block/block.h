#ifndef BLOCK_H
#define BLOCK_H

#include "../expression/symbol/symbol.h"
#include "../expression/symbol/function/function.h"
#include "../expression/symbol/variable/hidden_variable.h"
#include <list>

/*
Provides a block of multiple different symbols accross a scope.
*/
class Block final {
    public:
        Block() = default;
        ~Block() = default;
        /*
        Returns a read access to this Block symbols list.
        */
        std::list<Symbol*>      get_entries() const;
        /*
        Activates all of the symbols of this Block.
        */
        void                    Activate();
        /*
        Deactivates all the variables of this Block.
        */
        void                    Deactivate();   
        /*
        Searches for a symbol with an id on this Block.
        Returns a read/write access to a reference of the symbol if it's found,
        else nullptr.
        */
        Symbol*                 Lookup(std::string id);
        /*
        Searches for a function with an id on this Block.
        Returns a read/write access to a reference of the function if it's found,
        else nullptr.
        */
        Function*               LookupFunc(std::string id);
        /*
        Searches for the previously inserted function on this Block.
        Returns a read/write access to a reference of the function if it's found,
        else nullptr.
        */
        Function*               LookupPreviousFunc();
        /*
        Searches for a hidden variable with a name at this Block.
        Returns a read/write access to the reference of the hidden variable if it's found,
        else nullptr.
        */
        HiddenVariable*         LookupHiddenVariable(std::string name);
        /*
        Inserts a symbol at the end of this Block.
        */
        void                    Insert(Symbol* symbol);
        /*
        Returns wether this Block contains only hidden variables.
        */
        bool                    ContainsOnlyHiddenVars();

        friend std::ostream&    operator<<(std::ostream& os, const Block block);
    private:
       std::list<Symbol*> entries;

       static bool                     IsAFunctionEntry(Symbol* entry);
       static bool                     IsHiddenVariable(Symbol* entry);
};

#endif