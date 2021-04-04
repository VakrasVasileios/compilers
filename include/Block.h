#include <map>
#include <list>
#include <string>

#ifndef BLOCK
#define BLOCK

struct Block {
    std::list<std::string>  ids;    //mallon prepei na ginei map gt prepei na kratame kai to type
    unsigned int            scope;
    bool                    isActive;
    Block() = default;
    Block(std::list<std::string> _ids, unsigned int _scope, bool _isActive) : ids(_ids), scope(_scope), isActive(_isActive) {};

    std::list<std::string>::iterator     FindId(const std::string& _id);
};

#endif