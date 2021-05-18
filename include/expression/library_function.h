#ifndef LIBRARYFUNCTION_H
#define LIBRARYFUNCTION_H

#include "function.h"

/**
 * @brief A library function symbol expression.
 * Has a program var scope space.
**/ 
class LibraryFunction final : public Function {
public:
    LibraryFunction(std::string id, unsigned int line, unsigned int scope, unsigned int offset)
        : Function(LIB_FUNC, id, line, scope, PROGRAM_VAR, offset) {};

    LibraryFunction(std::string id, unsigned int line, unsigned int scope, unsigned int offset, std::list<FormalVariable*> formalArguments) :
        Function(LIB_FUNC, id, line, scope, PROGRAM_VAR, offset, formalArguments) {};

    ~LibraryFunction() = default;   

    std::ostream&                       LogSymbol(std::ostream& os) const override;    
};

#endif