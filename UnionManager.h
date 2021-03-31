#ifndef UNION_MANAGER
#define UNION_MANAGER

class UnionManager {
public:
    static auto     GetInstance(void) -> UnionManager { return instance; }
    
    void            IncreaseScope(void) { ++current_scope; }
    void            DecreaseScope(void) { --current_scope; }
    
    void            RegisterAnonimousFunc(/*something here*/);    // TODO: prepei na kanei assign onoma "$<anonimous_func_num>" kai na kanei increment ton
                                                // ton ari8mo
    void            RegisterFunc();

    void            LookUpVar(char* id);        // TODO
    void            LookUpFunc(char* id);       // TODO

private:
    static UnionManager instance;
    unsigned int current_scope = 0;
    unsigned int anonimous_func_num = 0;

    UnionManager() = default;
    ~UnionManager() = default;
};

#endif
