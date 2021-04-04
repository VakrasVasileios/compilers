#include "../include/BlockList.h"

Block*
BlockList::GetActiveBlock(void) {
    auto iter = begin();
    for (; iter != end(); iter++) {
        if ((*iter).isActive) return &(*iter);
    }
    return nullptr;
}

void
BlockList::ReActivateBlock(void) {
    if (block != nullptr)    block->isActive = true;
}
void
BlockList::DeactivateBlock(void) {
    block = GetActiveBlock();
    if (block != nullptr)    block->isActive = false;
}