#ifndef STMT_H
#define STMT_H

namespace target_code
{
    namespace intermediate_code 
    {
    /**
     * @brief A statement type.
     * 
     */
    enum StmtType {
        LOOP_t,
        IF_t,
        FUNC_t
    };
    /**
     * @brief A statement.
     * 
     */
    class Stmt {
    public:
        /**
         * @brief Constructs a new Stmt object.
         * 
         * @param _type the type of the statement.
         */
        Stmt(StmtType _type) : type(_type) {}
        /**
         * @brief Destroys this Stmt object.
         * 
         */
        virtual ~Stmt() = default;
        /**
         * @brief Returns a read access to the type of
         * this Stmt.
         * 
         * @return  a read access to the type of
         * this Stmt
         */
        StmtType    GetType() const;
    private:
        const StmtType type;
    };
    }
}

#endif