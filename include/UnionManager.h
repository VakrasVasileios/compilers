#ifndef UNION_MANAGER
#define UNION_MANAGER

#include <map>
#include <unordered_map>
#include <string>
#include <list>
#include <vector>
#include "Block.h"
#include "BlockList.h"
#include "SymbolTable.h"


#define global_scope  0

class UnionManager {
public:
    void            IncreaseScope(void) { ++current_scope; }
    void            DecreaseScope(void) { --current_scope; }
    
    void            RegisterAnonimousFunc(void);
    void            RegisterVar(const char* id);
    void            RegisterFunc(const char* id);
    
    bool            IsVarInActiveBlock(const char* id);
    bool            IsFuncInLibraryTableOrActiveBlock(const char* id);

    ~UnionManager() = default;
    UnionManager();
private:
    unsigned int        current_scope = 0;
    unsigned int        anonymous_func_num = 0;

    bool            IsFuncInActiveBlock(const char* id);
    bool            IsFuncInLibraryTable(const char* id);

    SymbolTable  variableTable;
    SymbolTable  functionTable;
    std::unordered_map<std::string, bool> libraryFuncTable;

    void            InitLibraryFuncs(void);

};

#endif


/*
    INSERT(var) {
        for (scope = global_scope to (current_scope -1)) {
            l = find_active_block_with_scope(scope);
            if(id_is_in_active_block(l)) then error;
        }
        insert_to_current_block(var, current_scope);
    }
*/
