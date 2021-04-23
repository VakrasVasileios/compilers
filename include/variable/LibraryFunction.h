#include "Variable.h"

#ifndef LIBRARYFUNCTION
#define LIBRARYFUNCTION

/*
Provides library function.
*/
class LibraryFunction : public Variable {
    public:
        /*
        The Parameterized Constructor of this LibraryFunction. Sets this LibraryFunction name, line, scope.
        */
        LibraryFunction(std::string name, unsigned int line, unsigned int scope) : Variable(LIB_FUNC, name, line, scope) {};
};

#endif