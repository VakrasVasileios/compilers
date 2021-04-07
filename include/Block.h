#include <map>
#include <string>
#include "Variable.h"

#ifndef BLOCK
#define BLOCK

struct Block {
    std::map<const std::string, Variable>  ids;
    Block() = default;
    Block(std::map<const std::string, Variable> _ids) : ids(_ids) {};

    void    SetIDVisibility(const std::string& _id, bool _visible);
    void    SetVisibilityAll(bool _visible);

    void    logBlock(void);
};

#endif