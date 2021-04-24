#include <string>
#include <iostream>

#ifndef ID
#define ID

/* 
Provides an abstract id.
*/
class Id {
    private:
        const std::string   name;
        const unsigned int  line;
        const unsigned int  scope;
    public:
        /*
        The Parameterized Constructor of this Id. Sets this Id name, line and scope .
        */   
        Id(std::string name, unsigned int line, unsigned int scope) : name(name), line(line), scope(scope) {}; 
        /*
        The Default Destructor of this Id.
        */   
        virtual                 ~Id() = default;
        /* 
        Returns the name of this Id.
        */
        std::string             getName() const;
        /* 
        Returns the line of this Id.
        */
        unsigned int            getLine() const;
        /* 
        Returns the scope of this Id.
        */
        unsigned int            getScope() const;

        friend std::ostream&    operator<<(std::ostream& os, const Id id);
};

#endif