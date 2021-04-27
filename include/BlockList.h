// #include <list>
// #include <stack>
// #include "symbol_table/block/Block.h"

// #ifndef BLOCK_LIST
// #define BLOCK_LIST

// class BlockList : public std::list<Block> {
// public:
//     void        resetVisibility(void);
//     void        setSymbolTableEntryVisibility(const SymbolTableEntry& symbolTableEntry, bool visible);

//     void        setCurrentBlockVisibility(bool visible);

//     auto        lookUp(const std::string name);
 
//     Block*      getCurrentBlock(void)   { return &*(--end()); }

//     void        logBlockList(void);
// };

// #endif