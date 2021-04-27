#include "SymbolTable.h"

void
SymbolTable:: insert(int scope, SymbolTableEntry entry){
    this->blocksTable[scope].back().addSymbolTableEntry(entry);
}



// SymbolTableEntry
// SymbolTable:: LookUp(SymbolTableEntry entry) {
//     // unsigned int entryScope = entry.getId().getScope();
//     // for (; entryScope >= 0; entryScope--)
//     // {
//     //     std::stack<SymbolTableEntry> temp;
//     //     while (!this->blocksMap[entryScope].empty())
//     //     {
//     //         if (entry != this->blocksMap[entryScope].top().getSymbolTableEntries())
//     //         {
//     //             /* code */
//     //         }
            
//     //     }
        
//     // }    
// }

// void
// SymbolTable:: Hide(int scope) {

// }

// void
// SymbolTable::logSymbolTable(void) {
//     for (int i = 0; i < (*this).size(); i++){
//         std::cout << std::endl;
//         std::cout << "-----------    Scope #" << i << "    -----------" << std::endl;
//         (*this)[i].logBlockList();
//     }
// }