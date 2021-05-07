#ifndef SYMBOL_H
#define SYMBOL_H

#include "../expression.h"
#include <string>
#include <iostream>

class Symbol : public Expression {
public:
    std::string             get_id() const;
    unsigned int            get_line() const;
    unsigned int            get_scope() const;
    bool                    is_active() const;
    void                    set_active(bool active);

    friend std::ostream&    operator<<(std::ostream& os, const Symbol* symbol);
protected:
    Symbol(ExprType type, std::string id, unsigned int line, unsigned int scope) :
    Expression(type), id(id), line(line), scope(scope), active(true) {};
    
    ~Symbol() = default;
      
    virtual std::ostream&   LogSymbol(std::ostream& os) const = 0;
    std::ostream&           LogExpression(std::ostream& os) const override; 
private:
    const std::string   id;
    const unsigned int  line;
    const unsigned int  scope;
    bool                active;
};

#endif