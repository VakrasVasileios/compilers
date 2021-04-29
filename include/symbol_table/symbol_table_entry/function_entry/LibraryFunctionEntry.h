#ifndef LIBRARYFUNCTIONENTRY_H
#define LIBRARYFUNCTIONENTRY_H

#include "FunctionEntry.h"

/*
Provides a library function entry.
*/
class LibraryFunctionEntry final : public FunctionEntry {
    public:
        /*
        The Parameterized Constructor of this LibraryFunctionEntry.
        */
        LibraryFunctionEntry(std::string id, unsigned int line, unsigned int scope) : FunctionEntry(LIB_FUNC, id, line, scope) {};
        /*
        The Parameterized Constructor of this LibraryFunctionEntry. Sets this LibraryFunctionEntry formal arguments.
        */
        LibraryFunctionEntry(std::string id, unsigned int line, unsigned int scope, std::list<FormalVariableEntry> formalArguments) :
            FunctionEntry(LIB_FUNC, id, line, scope) {};
};

#endif