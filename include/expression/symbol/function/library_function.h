#ifndef LIBRARYFUNCTION_H
#define LIBRARYFUNCTION_H

#include "function.h"

/*
Provides a library function.
*/
class LibraryFunction final : public Function {
public:
    LibraryFunction(std::string id, unsigned int line, unsigned int scope) : Function(LIB_FUNC, id, line, scope) {};

    LibraryFunction(std::string id, unsigned int line, unsigned int scope, std::list<FormalVariable> formalArguments) :
        Function(LIB_FUNC, id, line, scope) {};

    ~LibraryFunction() = default;    
};

#endif