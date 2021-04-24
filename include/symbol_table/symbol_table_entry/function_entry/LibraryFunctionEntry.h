#include "include/symbol_table/symbol_table_entry/function_entry/FunctionEntry.h"

#ifndef LIBRARYFUNCTIONENTRY
#define LIBRARYFUNCTIONENTRY

/*
Provides a library function entry.
*/
class LibraryFunctionEntry final : public FunctionEntry {
    public:
        /*
        The Parameterized Constructor of this LibraryFunctionEntry. Sets this LibraryFunctionEntry formal arguments.
        */
        LibraryFunctionEntry(Function function, std::list<FormalVariableEntry> formalArguments) : FunctionEntry(LIB_FUNC, function, formalArguments) {};
};

#endif