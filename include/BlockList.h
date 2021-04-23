// #include <list>
// #include <stack>
// #include "Block.h"

// #ifndef BLOCK_LIST
// #define BLOCK_LIST

// class BlockList : public std::list<Block> {
// public:
//     void        ResetVisibility(void);
//     void        SetIDVisibility(const std::string& _id, bool _visi);

//     void        SetCurrentBlockVisibility(bool _visible);

//     auto        LookUp(const std::string& _id) -> std::pair<const std::string, Variable>*;
 
//     Block*      GetCurrentBlock(void)   { return &*(--end()); }

//     void        logBlockList(void);
// };

// #endif