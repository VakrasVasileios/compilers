#include <string>
#include <iostream>

#ifndef VARIABLE
#define VARIABLE

enum type_t {
    LIB_FUNC = 0,
    USER_FUNC = 1,
    LOCAL_VAR = 2,
    GLOBAL_VAR = 3,
    FORMAL_ARG = 4
};

struct Variable {
    type_t type;
    bool isVisible = true;
    unsigned int line;
    unsigned int scope;

    Variable() = default;
    Variable(type_t _type, bool _visible, unsigned int _line, unsigned int _scope) : type(_type), isVisible(_visible), line(_line), scope(_scope) {};

    friend std::ostream&    operator<<(std::ostream& os, const Variable& v);
};

#endif

