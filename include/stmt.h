#ifndef STMT_H
#define STMT_H

enum StmtType {
    LOOP_t,
    IF_t,
    FUNC_t
};

class Stmt {
public:
    Stmt(StmtType _type) : type(_type) {}
    virtual ~Stmt() = default;

    StmtType    GetType() const;

private:
    const StmtType type;

};

#endif