#include "../include/block_stack/BlockStack.h"
#include <algorithm>

Block*
BlockStack:: topBlock() {
    return !blockStack.empty() ? *(--blockStack.end()) : nullptr;
}

void
BlockStack:: pushBlock(Block* block) {
    blockStack.push_back(block);
}

void
BlockStack:: popBlock() {
    (*(--blockStack.end()))->hide();
    blockStack.pop_back();
}

SymbolTableEntry*
BlockStack:: Lookup(const char* name) {
    auto iter = blockStack.end();
    do {
        --iter;
        auto b = (*iter)->getSymbolTableEntries();
        for (auto i : b) {
            if (i.getId().c_str() == name)
                return &i; 
        }
    } while (iter != blockStack.begin());

    return nullptr;
}

SymbolTableEntry*   LookupGlobal(const char* name) {
    auto iter = blockStack.begin();
    auto b = (*iter)->getSymbolTableEntries();
    for (auto i : b) {
        if (i.getId().c_str() == name)
            return &i; 
    }

    return nullptr;
}

void
BlockStack:: hideLowerScopes() {
    auto iter = blockStack.begin();
    for (; iter != blockStack.end(); ++iter) {
        for (auto entry : (*iter)->getSymbolTableEntries())
        {
            entry.setActive(false);
        }
    }
}

void
BlockStack:: enableLowerScopes() {
    auto iter = blockStack.begin();
    for (; iter != blockStack.end(); ++iter) {
        for (auto entry : (*iter)->getSymbolTableEntries())
        {
            entry.setActive(true);
        }
    }
}
