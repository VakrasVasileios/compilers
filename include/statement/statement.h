#ifndef STATEMENT_H
#define STATEMENT_H


/**
 * @brief The type of a statement.
**/
enum StmtType {
    Loop_t,
    FUNC_DEF_t
};

/**
 * @brief An abstract statement.
**/
class Statement {
public:
    /**
     * @brief Returns a read access to the type of this Statement.
     * 
     * @return a read access to the type of this Statement, not null
    **/ 
    Statement       get_type() const;   
protected:
    Statement(StmtType loop_type) : type(type) {};
    ~Statement() = default;
private:
    const StmtType  type;
};

#endif

