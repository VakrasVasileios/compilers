#include "../include/Block.h"

std::list<std::string>::iterator
Block::FindId(const std::string& _id) {
    auto iter = ids.begin();
    for (; iter != ids.end(); iter++) {
        if (*iter == _id)   return iter;
    }
    return ids.end();
}
