#include <string>
#include <iostream>

#ifndef VARIABLE
#define VARIABLE

/* 
Provides a constant type for a Variable object.
*/
enum type_t {
    /*
    The type of a Library Function.
    */
    LIB_FUNC,
    /*
    The type of a User Function.
    */
    USER_FUNC,
    /*
    The type of a Local Variable.
    */
    LOCAL_VAR,
    /*
    The type of a Global Variable.
    */
    GLOBAL_VAR,
    /*
    The type of a Formal Argument.
    */
    FORMAL_ARG
};

/* 
Provides an abstract variable.
*/
class Variable {
    private:
        const type_t        type;
        const std::string   name;
        const unsigned int  line;
        const unsigned int  scope;
    public:
        /* 
        The Parameterized Constructor of this Variable. Sets this Variable type, name, line and scope .
        */
        Variable(type_t type, std::string name, unsigned int line, unsigned int scope) : type(type), name(name), line(line), scope(scope) {};
        /*
        The Default Constructor of this Variable.
        */
        virtual                 ~Variable() = default;
        /* 
        Returns the type of this Variable.
        */
        type_t                  getType() const;
        /* 
        Returns the name of this Variable.
         */
        std::string             getName() const;
        /* 
        Returns the line of this Variable.
        */
        unsigned int            getLine() const;
        /* 
        Returns the scope of this Variable.
        */
        unsigned int            getScope() const;

        friend std::ostream&    operator<<(std::ostream& os, const Variable variable);
};

#endif
// enum type_t {
//     LIB_FUNC = 0,
//     USER_FUNC = 1,
//     LOCAL_VAR = 2,
//     GLOBAL_VAR = 3,
//     FORMAL_ARG = 4
// };

// struct Variable {
//     type_t type;
//     bool isVisible = true;
//     unsigned int line;
//     unsigned int scope;

//     Variable() = default;
//     Variable(type_t _type, bool _visible, unsigned int _line, unsigned int _scope) : type(_type), isVisible(_visible), line(_line), scope(_scope) {};

//     friend std::ostream&    operator<<(std::ostream& os, const Variable& v);
// };



