#include <list>
#include "Block.h"

#ifndef BLOCK_LIST
#define BLOCK_LIST

class BlockList : public std::list<Block> {
public:
    Block*  GetActiveBlock(void);

    void    ReActivateBlock(void);
    void    DeactivateBlock(void);

private:
    Block* block;
};

#endif