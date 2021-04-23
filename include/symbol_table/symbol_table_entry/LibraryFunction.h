#include "include/symbol_table/symbol_table_entry/SymbolTableEntry.h"
#include "include/id/Function.h"

#ifndef LIBRARYFUNCTION
#define LIBRARYFUNCTION

/*
Provides a library function.
*/
class LibraryFunction final : public SymbolTableEntry {
    public:
        /*
        The Parameterized Constructor of this LibraryFunction. Sets this LibraryFunction function.
        */
        LibraryFunction(Function function) : SymbolTableEntry(LIB_FUNC, function, true) {};
};

#endif