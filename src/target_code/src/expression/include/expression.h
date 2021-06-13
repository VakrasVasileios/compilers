#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>
#include <list>
#include "../../../../../util/contract/include/contract.h"

namespace target_code 
{
    namespace expression
    {
    class ExpressionVisitor; // fwd declare

    /**
     * @brief The type of an expression.
     * 
    **/ 
    enum ExprType {
        ASSIGN,
        BOOL,
        ARITHMETIC,
        
        LIB_FUNC,
        USER_FUNC,
        VAR,
        CONST_INT,
        CONST_DOUBLE,
        CONST_BOOL,
        CONST_STR,
        CONST_NIL,
        CALL,
        TABLE_ITEM,
        TABLE_MAKE
    };

    /**
     * @brief An expression abstraction.
     * 
    **/ 
    class Expression {
    public:
        /**
         * @brief Returns a read access to this Expression type.
         * 
         * @return a read access to this Expression type.
        **/ 
        ExprType                get_type() const;

        friend std::ostream&    operator << (std::ostream& os,
            const Expression* expression);
        /**
         * @brief Returns a read access to this Expression as a 
         * string.
         * 
         * @return a read access to this Expression as a 
         * string.
         */
        virtual std::string     to_string() const = 0;
        /**
         * @brief Returns a read access to this Expression
         * as an unsigned int.
         * 
         * @return a read access to this Expression
         * as an unsigned int
         */
        virtual unsigned int    to_unsigned() const = 0;
        /**
         * @brief Accepts an expression visitor at this
         * Expression.
         * 
         * @param expr_visitor the expression visitor to be accepted,
         * not null
         */
        virtual void            Accept(ExpressionVisitor* expr_visitor) = 0;
    protected:
        Expression(ExprType _type) : type(_type) {};
        ~Expression() = default;
        /**
         * @brief Logs this Expression.
         * 
         * @param os the ostream to output this Expression.
         * 
        **/ 
        virtual std::ostream&   LogExpression(std::ostream& os) const = 0;
    private:
        ExprType type;
    };
    /**
     * @brief Checks wether an expression is a 
     * library function.
     * 
     * @param expr the expression to be checked, not null
     * 
     * @return wether an expression is a 
     * library function
     */
    bool                        IsLibraryFunction(Expression* expr);
    /**
     * @brief Checks wether an expression is a 
     * user function.
     * 
     * @param expr the expression to be checked, not null
     * 
     * @return wether an expression is a 
     * user function
     */
    bool                        IsUserFunction(Expression* expr);
    /**
     * @brief Checks wether an expression is a 
     * variable.
     * 
     * @param expr the expression to be checked, not null
     * 
     * @return wether an expression is a 
     * variable
     */
    bool                        IsVariable(Expression* expr);
    /**
     * @brief Checks wether an expression is a 
     * constant boolean.
     * 
     * @param expr the expression to be checked, not null
     * 
     * @return wether an expression is a 
     * constant boolean.
     */
    bool                        IsConstBool(Expression* expr);
    /**
     * @brief Checks wether an expression is a 
     * constant string.
     * 
     * @param expr the expression to be checked, not null
     * 
     * @return wether an expression is a 
     * constant string.
     */
    bool                        IsConstString(Expression* expr);
    /**
     * @brief Checks wether an expression is a 
     * table make.
     * 
     * @param expr the expression to be checked, not null
     * 
     * @return wether an expression is a 
     * table make.
     */
    bool                        IsTableMake(Expression* expr);

    /**
     * @brief A binary expression.
     * 
     */
    class BinaryExpr : public Expression {
    public:
        /**
         * @brief Returns a read access to this BinaryExpr result.
         * 
         * @return a read access to this BinaryExpr result, not null 
         */
        Expression*     get_result() const;
        /**
         * @brief Returns a read access to this BinaryExpr left
         * hand operand expression.
         * 
         * @return a read access to this BinaryExpr left
         * hand operand expression, not null.
         */
        Expression*     get_left_hand_operand() const;
        /**
         * @brief Returns a read access to this BinaryExpr right
         * hand operand expression.
         * 
         * @return a read access to this BinaryExpr right
         * hand operand expression
         */
        Expression*     get_right_hand_operand() const;

        std::string     to_string() const override;
        unsigned int    to_unsigned() const override;
    protected:
        /**
         * @brief Constructs a new BinaryExpr object.
         * 
         * @param type the expression type of the BinaryExpr object,
         * must be ARITHMETIC, BOOL or ASSIGN.
         * @param result the result of the BinaryExpr object, not null
         * @param left_hand_operand the left hand operand of the BinaryExpr
         * object, not null
         * @param right_hand_operand the right hand operand of the BinaryExpr
         * object, not null
         */
        BinaryExpr(ExprType type, Expression* result,
            Expression* left_hand_operand, Expression* right_hand_operand)
            : Expression(type), result(verify_result(result)),
                left_hand_operand(left_hand_operand),
                    right_hand_operand(right_hand_operand) {};
        /**
         * @brief Destroys this BinaryExpr object.
         * 
         */
        ~BinaryExpr() = default;
    private:
        Expression*     result;
        Expression*     left_hand_operand;
        Expression*     right_hand_operand; 

        ExprType        verify_type(ExprType type);
        Expression*     verify_operand(Expression* operand);  
        Expression*     verify_result(Expression* result);   

        std::ostream&   LogExpression(std::ostream& os) const override;
    };

    /**
     * @brief An arithmetic binary expression.
     * 
     */
    class ArithmeticExpr final : public BinaryExpr {
    public:
        /**
         * @brief Constructs a new ArithmeticExpr object
         * 
         * @param result the result of this ArithmeticExpr operand, not null
         * @param left_hand_operand the left hand operand of this ArithmeticExpr
         * operand, not null 
         * @param right_hand_operand the right hand operand of this 
         * ArithmeticExpr operand, not null 
         */
        ArithmeticExpr(Expression* result, Expression* left_hand_operand,
            Expression* right_hand_operand)
            : BinaryExpr(ARITHMETIC, result, left_hand_operand,
                right_hand_operand) {};
        /**
         * @brief Destroys this ArithmeticExpr object.
         * 
         */
        ~ArithmeticExpr() = default; 
        void            Accept(ExpressionVisitor* expr_visitor) override;   
    };

    /**
     * @brief An assign binary expression.
     * 
     */
    class AssignExpr final : public BinaryExpr {
    public:
        /**
         * @brief Constructs a new AssignExpr object
         * 
         * @param result the result of this AssignExpr operand, not null 
         * @param left_hand_operand the left hand operand of this AssignExpr
         * operand, not null 
         * @param right_hand_operand    the right hand operand of this
         * AssignExpr operand, not null 
         */
        AssignExpr(Expression* result, Expression* left_hand_operand,
            Expression* right_hand_operand)
        : BinaryExpr(ASSIGN, result, left_hand_operand, right_hand_operand) {};
        /**
         * @brief Destroys this AssignExpr object.
         * 
         */
        ~AssignExpr() = default;
        void            Accept(ExpressionVisitor* expr_visitor) override;
    };

    /**
     * @brief A boolean logical binary expression.
     * 
     */
    class BoolExpr final : public BinaryExpr {
    public:
        /**
         * @brief Constructs a new Bool Expr object.
         * 
         * @param result the result of this BoolExpr, not null
         * @param left_hand_operand the left hand operand of the BoolExpr
         * object, not null
         * @param right_hand_operand the right hand operand of the BoolExpr
         * object, not null
         */
        BoolExpr(Expression* result, Expression* left_hand_operand,
            Expression* right_hand_operand)
            : BinaryExpr(BOOL, result, left_hand_operand, right_hand_operand) 
            {};
        /**
         * @brief Destroys this Bool Expr object.
         * 
         */
        ~BoolExpr() = default;
        /**
         * @brief Holds the labels of the jump/branch quads that will
         * be concluded to 'true' during the short circuit conclusion.
         * 
         */
        std::list<unsigned int> true_list;
        /**
         * @brief Holds the labels of the jump/branch quads that will
         * be concluded to 'false' during the short circuit conclusion.
         * 
         */
        std::list<unsigned int> false_list;
        void            Accept(ExpressionVisitor* expr_visitor) override;
    };

    /**
     * @brief A primary expression.
     * 
     */
    class Primary : public Expression {
    protected:
        /**
         * @brief Constructs a new Primary object.
         * 
         * @param type the expression type of this
         * Primary object, must be LIB_FUNC, USER_FUNC,
         * VAR, TABLE_MAKE, TABLE_ITEM, CALL, CONST_NUM,
         *  CONST_BOOL, or CONST_NIL.
         */
        Primary(ExprType type) : Expression(verify_type(type)) {};
        /**
         * @brief Destroys this Primary object.
         * 
         */
        ~Primary() = default; 
    private:
        ExprType    verify_type(ExprType type);    
    };

    /**
     * @brief A constant primary expression.
    **/ 
    class Constant : public Primary {
    protected:
        Constant(ExprType type) : Primary(type){};
        ~Constant() = default;         
    };

    /**
     * @brief A bool constant expression.
    **/ 
    class BoolConstant final : public Constant {
    public:
        BoolConstant(bool value) : Constant(CONST_BOOL), value(value) {};
        ~BoolConstant() = default;
        /**
         * @brief Returns a read access to this BoolConstant value.
         * 
         * @return a read access to this BoolConstant value.
        **/ 
        bool            get_value() const;
        std::string     to_string() const override;
        unsigned int    to_unsigned() const override;
        void            Accept(ExpressionVisitor* expr_visitor) override;
    protected:
        std::ostream&   LogExpression(std::ostream& os) const override;     
    private:
        bool            value;   
    };

    /**
     * @brief A numeric constant expression.
    **/ 
    class NumericConstant: public Constant {
    protected:
        NumericConstant(ExprType _type) : Constant(_type) {};
        ~NumericConstant() = default;
    };

    class DoubleConstant final : public NumericConstant {
    public:
        DoubleConstant(double value) : NumericConstant(CONST_DOUBLE),
            value(value) {};
        ~DoubleConstant() = default;
        /**
         * @brief Returns a read access to this DoubleConstant value.
         * 
         * @return a read access to this DoubleConstant value
        **/ 
        double          get_value() const;
        std::string     to_string() const override;
        unsigned int    to_unsigned() const override;
        void            Accept(ExpressionVisitor* expr_visitor) override;
    protected:
        std::ostream&   LogExpression(std::ostream& os) const override;     
    private:
        double value;    
    };

    /**
     * @brief An int constant expression.
     * 
    **/ 
    class IntConstant final : public NumericConstant {
    public:
        IntConstant(int value) : NumericConstant(CONST_INT), value(value) {};
        ~IntConstant() = default;
        /**
         * @brief Returns a read access to this IntConstant value.
         * 
         * @return a read access to this IntConstant value
        **/ 
        int             get_value() const;
        std::string     to_string() const override;
        unsigned int    to_unsigned() const override;
        void            Accept(ExpressionVisitor* expr_visitor) override;
    protected:
        std::ostream&   LogExpression(std::ostream& os) const override;     
    private:
        int value;    
    };

    /**
     * @brief A string constant expression.
    **/ 
    class StringConstant final : public Constant {
    public:
        StringConstant(std::string value) : Constant(CONST_STR),
            value(value) {};
        ~StringConstant() = default;
        /**
         * @brief Returns a read access to this StringConstant value.
        **/ 
        std::string     get_value() const;
        std::string     to_string() const override;
        unsigned int    to_unsigned() const override;
        void            Accept(ExpressionVisitor* expr_visitor) override;
    protected:
        std::ostream&   LogExpression(std::ostream& os) const override; 
    private:
        std::string     value;
    };

    /**
     * @brief A NILL constant expression.
    **/ 
    class NilConstant final : public Constant {
    public:
        NilConstant(std::nullptr_t value) : Constant(CONST_NIL),
            value(value) {};
        ~NilConstant() = default;
        /**
         * @brief Returns a read access to this NilConstant value.
         * 
         * @return a read access to this NilConstant value, null
         * 
        **/ 
        std::nullptr_t  get_value() const;
        std::string     to_string() const override;
        unsigned int    to_unsigned() const override;
        void            Accept(ExpressionVisitor* expr_visitor) override;
    protected:
        std::ostream&   LogExpression(std::ostream& os) const override;    
    private:
        std::nullptr_t  value;    
    };

    /**
     * @brief The scope space of a symbol.
     * Indicates wether a symbol is declared inside/outside of a function,
     * or as a function argument.
    **/ 
    enum ScopeSpace {
        PROGRAM_VAR,    // out of functions
        FUNCTION_LOCAL, // inside of function
        FORMAL_ARG
    };
    /**
     * @brief An lvalue primary expression.
     * 
    **/ 
    class Symbol final : public Primary {
    public:
        /**
         * @brief Constructs a new Symbol object.
         * 
         * @param type the expression type of this Symbol, must be of LIB_FUNC,
         * USER_FUNC, VAR type, or TABLE_ITEM
         * @param id the unique identifier of this Symbol, not empty
         * @param line the line of this Symbol, greater or equal to zero.
         * @param scope the scope of this Symbol, greater or equal to zero.
         * @param space the scope space of this Symbol, indicating wether this
         * Symbol has been declared inside/outside of a function definition, or
         * as a formal argument.
         * @param offset the offset of this Symbol at its scope space, greater
         * or equal to zero.
         * @param index the mapped index to this Symbol, at a TABLE_ITEM
         * context.
         */
        Symbol(ExprType type, std::string id, unsigned int line,
            unsigned int scope , ScopeSpace space, unsigned int offset,
                Expression* index) :
            Primary(verify_type(type)), id(verify_id(id)),
                line(verify_line(line)), scope(verify_scope(scope)),
                    active(true), space(space), offset(verify_offset(offset)),
                        index(verify_index(type, index)) {};
        /**
         * @brief Destroys this Symbol object.
         * 
         */
        ~Symbol() = default;
        /**
         * @brief Returns a read access to this Symbol id.
         * 
         * @return a read access to this Symbol id, not null
        **/ 
        std::string                 get_id() const;
        /**
         * @brief Returns a read access to this Symbol line.
         * 
         * @return a read access to this Symbol line, greater or equal to zero.
         * 
        **/ 
        unsigned int                get_line() const;
        /**
         * @brief Returns a read access to this Symbol scope.
         * 
         * @return a read access to this Symbol scope, greater or equal to zero.
         *
        **/  
        unsigned int                get_scope() const;
        /**
         * @brief Returns a read access to this Symbol activity.
         * Determines wether this Symbol is visible or not at this occurance. 
         * 
         * @return a read access to this Symbol activity
        **/ 
        bool                        is_active() const;
        /**
         * @brief Returns a read access to this Symbol scope space.
         * The scope space of a symbol indicates wether it's declared
         * inside/outside of a function, or as a formal argument.
         * 
         * @return a read access to this Symbol scope space
        **/ 
        ScopeSpace                  get_space() const;
        /**
         * @brief Returns a read access to this this Symbol offset.
         * The offset of a symbol indicates its rank on its specific scope
         * space.
         * 
         * @return a read access to this this Symbol offset, greater or equal
         * to zero.
         * 
        **/ 
        unsigned int                get_offset() const;
        /**
         * @brief Returns a read access to this Symbol formal arguments.
         * 
         * @return a read access to this Symbol formal arguments, 
         * all have a FORMAL_ARG scope space.
         */
        std::list<Symbol*>          get_formal_arguments() const;
        /**
         * @brief Returns a read access to the mapped index of
         * this TABLE_ITEM symbol. 
         * It's a checked runtime error for this Symbol not tohave
         * a TABLE_ITEM expression type.
         * 
         * @return a read access to the mapped index of
         * this TABLE_ITEM symbol, can be null
         */
        Expression*                 get_index() const;

        unsigned int                get_taddress() const;

        unsigned int                get_total_local() const;
        /**
         * @brief Sets this Symbol activity.
         * A symbol's activity determines wether the Symbol
         * is visible or not at this occurance. 
         * 
         * @param active the activity to set this Symbol.
        **/ 
        void                        set_active(bool active);

        void                        set_taddress(unsigned _taddr);

        void                        set_total_local(unsigned int _lc);
        /**
         * @brief Checks wether this Symbol has already
         * included a formal argument to its list of
         * formal arguments.
         * 
         * @param formal_arg the formal argument to be checked, not null
         * and with a FORMAL_ARG scope space.
         *  
         * @return wether this Symbol has already
         * included a formal argument to its list of
         * formal arguments
         */
        bool                        HasFormalArg(Symbol* formal_arg); 
        /**
         * @brief Adds a formal argument to this Symbol list of
         * formal arguments. 
         * 
         * @param formal_arg the formal argument to be checked, not null,
         * with a FORMAL_ARG scope space and not already included to this
         * Symbol list of formal arguments.
         */
        void                        AddFormalArg(Symbol* formal_arg);                   

        friend std::ostream&        operator<<(std::ostream& os,
            const Symbol* symbol);    
        std::string                 to_string() const override;
        unsigned int                to_unsigned() const override;
        void                        Accept(ExpressionVisitor* expr_visitor)
                                        override;
    private:
        const std::string           id;
        const unsigned int          line;
        const unsigned int          scope;
        bool                        active;
        const ScopeSpace            space;
        const unsigned int          offset;
        unsigned int                total_local;
        unsigned int                taddress;
        std::list<Symbol*>          formal_arguments;
        Expression*                 index;

        ExprType                    verify_type(ExprType type);
        std::string                 verify_id(std::string id);
        unsigned int                verify_line(unsigned int line);
        unsigned int                verify_scope(unsigned int scope);
        unsigned int                verify_offset(unsigned int offset);
        Expression*                 verify_index(ExprType type,
                                        Expression* index);
        bool                        is_state_valid();

        std::string                 space_to_string() const;
        std::string                 sym_to_string() const;

        std::ostream&               LogExpression(std::ostream& os) const
                                        override;
    };
    /**
     * @brief Checks wether a symbol expression is
     * a table item.
     * 
     * @param symbol the symbol expression to be
     * checked, not null
     * 
     * @return wether a symbol expression is
     * a table item 
     */
    bool                            IsTableItem(Symbol* symbol);

    /**
     * @brief An expression list.
     * 
     */
    class Elist {
    public:
        /**
         * @brief Constructs a new Elist object.
         * 
         */
        Elist() = default;
        /**
         * @brief Destroys this Elist object.
         * 
         */
        ~Elist() = default;
        /**
         * @brief This EList list of expressions.
         * 
         */
        std::list<Expression*>  exprs;
    };

    /**
     * @brief The call suffix type.
     * 
     */
    enum CallSuffixType {
        NORM_CALL,
        METHOD_CALL
    };

    /**
     * @brief A call suffix.
     * 
     */
    class CallSuffix {
    public:
        /**
         * @brief Returns a read access to this CallSuffix
         * type.
         * 
         * @return a read access to this CallSuffix
         * type
         */
        CallSuffixType get_type() const;
        /**
         * @brief Returns a read access to this CallSuffix
         * name.
         * 
         * @return a read access to this CallSuffix
         * name 
         */
        std::string get_name() const;
        /**
         * @brief Returns a read access to this CallSuffix
         * elist.
         * 
         * @return a read access to this CallSuffix
         * elist 
         */
        Elist*      get_elist() const; 
        /**
         * @brief Inserts an argument to this CallSuffix
         * elist.
         * 
         * @param arg the argument to be inserted to this 
         * CallSuffix elist, not null
         */
        void        InsertArg(Expression* arg);  
    protected:
        /**
         * @brief Constructs a new CallSuffix object.
         * 
         * @param type the CallSuffix type
         * @param name the name of the CallSuffix
         * @param elist the elist of the CallSuffix
         */
        CallSuffix(CallSuffixType type, std::string name, Elist* elist)
        : name(name), type(type), elist(elist) {};
        /**
         * @brief Destroys this Call Suffix object.
         * 
         */
        ~CallSuffix() = default;
    private:
        CallSuffixType          type;
        std::string             name;
        Elist*                  elist;
    };
    /**
     * @brief Checks wether a call suffix is
     * of method call type.
     * 
     * @param call_suffix the call suffix to be checked,
     * not null
     * 
     * @return wether a call suffix is
     * of method call type 
     */
    bool            IsMethodCall(CallSuffix* call_suffix);

    /**
     * @brief A method callsuffix.
     * 
     */
    class MethodCall final : public CallSuffix {
    public:
        /**
         * @brief Constructs a new MethodCall object.
         * 
         * @param name 
         * @param elist 
         */
        MethodCall(std::string name, Elist* elist)
        : CallSuffix(METHOD_CALL, name, elist) {};
        /**
         * @brief Destroys this MethodCall object.
         * 
         */
        ~MethodCall() = default;
    };

    /**
     * @brief A normal callsuffix.
     * 
     */
    class NormCall final : public CallSuffix {
    public:
        /**
         * @brief Constructs a new NormCall object.
         * 
         * @param elist 
         */
        NormCall(Elist* elist)
        : CallSuffix(NORM_CALL, std::string(""), elist) {};
        /**
         * @brief Destroys this NormCall object.
         * 
         */
        ~NormCall() = default;
    };

    /**
     * @brief A call primary expression.
    **/ 
    class Call final : public Primary {
    public:
        /**
         * @brief Constructs a new Call object.
         * 
         * @param called_symbol the called symbol of this Call, not null
         * @param return_val the return value of this call, not null
         */
        Call(Symbol* called_symbol, CallSuffix* call_suffix, Symbol* return_val)
        : Primary(CALL), called_symbol(verify_called_symbol(called_symbol)),
        call_suffix(verify_call_suffix(call_suffix)),
            ret_val(verify_ret_val(return_val)) {};
        /**
         * @brief Destroys this Call object.
         * 
         */
        ~Call() = default;
        /**
         * @brief Returns a read access to this Call called symbol.
         * 
         * @return a read access to this Call called symbol, can be null
        **/ 
        Symbol*                get_called_symbol() const;
        /**
         * @brief Returns a read access to this Call call suffix. 
         * 
         * @return a read access to this Call call suffix, not null 
         */
        CallSuffix*             get_call_suffix() const;
        /**
         * @brief Returns a read access to this Call return value.
         * 
         * @return a read access to this Call return value, can be null
        **/ 
        Symbol*                 get_ret_val() const;
        
        std::string             to_string() const override;
        unsigned int            to_unsigned() const override;
        void                    Accept(ExpressionVisitor* expr_visitor)
                                    override;
    protected:
        std::ostream&           LogExpression(std::ostream& os) const override;
    private:
        Symbol*                 called_symbol; 
        CallSuffix*             call_suffix;
        Symbol*                 ret_val;

        Symbol*                 verify_called_symbol(Symbol* called_symbol);
        CallSuffix*             verify_call_suffix(CallSuffix* call_suffix);
        Symbol*                 verify_ret_val(Symbol* ret_val);
    };

    /**
     * @brief An indexed list of indexed elements.
     * 
     */
    class Indexed final {
    public:
        /**
         * @brief Constructs a new Indexed object.
         * 
         */
        Indexed() = default;
        /**
            * @brief Destroys this Indexed object.
            * 
            */
        ~Indexed() = default;
        /**
            * @brief This Indexed list of indexed elements.
            * 
            */
        std::list<IndexedElem*> pairs;
    };

    /**
     * @brief An indexed element of a pair of expressions.
     * 
     */
    class IndexedElem final {
    public:
        /**
         * @brief Constructs a new IndexedElem object.
         * 
         * @param pair the indexed element pair of expressions
         */
        IndexedElem(std::pair<Expression*, Expression*>  pair) : pair(pair) {};
        /**
         * @brief Destroys this IndexedElem object.
         * 
         */
        ~IndexedElem() = default;
        /**
         * @brief The indexed element pair of expressions.
         * 
         */
        std::pair<Expression*, Expression*>  pair;
    };

    /**
     * @brief A table make primary expression.
     * 
     */
    class TableMake : public Primary {
    public:
        /**
         * @brief Returns a read access to this TableMake created table.
         * 
         * @return a read access to this TableMake created table, not null
         */
        Symbol*         get_table() const;
        std::string     to_string() const override;
        unsigned int    to_unsigned() const override;
    protected:
        /**
         * @brief Constructs a new TableMake object.
         * 
         */
        TableMake(Symbol* table) : Primary(TABLE_MAKE),
            table(verify_table(table)) {};
        /**
         * @brief Destroys this TableMakeobject.
         * 
         */
        ~TableMake() = default;
        std::ostream&   LogExpression(std::ostream& os) const override; 
    private:
        Symbol*         table;
        Symbol*         verify_table(Symbol* table);
    };

    /**
     * @brief A table make statement for storing expressions.
     * 
     */
    class TableMakeElems final : public TableMake {
    public:
        /**
         * @brief Constructs a new TableMakeElems object.
         * 
         */
        TableMakeElems(Symbol* table, Elist* elist) : TableMake(table),
            elements(verify_elements(elist)) {};
        /**
         * @brief Destroys this TableMakeElems object.
         * 
         */
        ~TableMakeElems() = default;
        /**
         * @brief Returns a read access to this TableMakeElems list
         * of inserted elements.
         * 
         * @return a read access to this TableMakeElems list
         * of inserted elements
         */
        Elist*  get_elements() const;
        void    Accept(ExpressionVisitor* expr_visitor) override;
    private:
        Elist*  elements;
        Elist*  verify_elements(Elist* elements);
    };

    /**
     * @brief A table make statement for storing pairs of expressions.
     * 
     */
    class TableMakePairs final : public TableMake {
    public:
        /**
         * @brief Constructs a new TableMakePairs object.
         * 
         * @param table the table of this TableMakePairs, not null 
         * @param indexed the indexed list of indexed elements of this
         * TableMakePairs, not null 
         */
        TableMakePairs(Symbol* table, Indexed* indexed): TableMake(table),
            indexed(verify_indexed(indexed)) {};
        /**
         * @brief Destroys this TableMakePairs object.
         * 
         */
        ~TableMakePairs() = default;
        /**
         * @brief Returns a read access to this TableMakePairs
         * indexed list of indexed elements.
         * 
         * @return  a read access to this TableMakePairs
         * indexed list of indexed elements, not null
         */
        Indexed*    get_indexed() const;
        void        Accept(ExpressionVisitor* expr_visitor) override;
    private:
        Indexed*    indexed;
        Indexed*    verify_indexed(Indexed* indexed);
    };

    /**
     * @brief An interface for visiting expression
     * concrete extensions.
     * 
     */
    class ExpressionVisitor {
    public:
        /**
         * @brief Visits an arithmetic expression.
         * 
         * @param arithm_expr the arithmetic expression to be visited,
         * not null
         */
        virtual void VisitArithmeticExpression(ArithmeticExpr* arithm_expr) = 0;
        /**
         * @brief Visits an assignment expression.
         * 
         * @param assign_expr the assignment expression to be visited,
         * not null
         */
        virtual void VisitAssignExpr(AssignExpr* assign_expr) = 0;
        /**
         * @brief Visits a boolean expression
         * 
         * @param bool_expr the boolean expression to be visited,
         * not null  
         */
        virtual void VisitBoolExpr(BoolExpr* bool_expr) = 0;
        /**
         * @brief Visits a call expression.
         * 
         * @param call the call expression to be visited,
         * not null
         */
        virtual void VisitCall(Call* call) = 0;
        /**
         * @brief Visits a double constant expression.
         * 
         * @param double_const the double constant expression to be visited,
         * not null
         */
        virtual void VisitDoubleConstant(DoubleConstant* double_const) = 0;
        /**
         * @brief Visits an int constant expression.
         * 
         * @param int_const the int constant expression to be visited,
         * not null
         */
        virtual void VisitIntConstant(IntConstant* int_const) = 0;
        /**
         * @brief Visits a nill constant expression.
         * 
         * @param nil_const the nil constant expression to be visited,
         * not null
         */
        virtual void VisitNilConstant(NilConstant* nil_const) = 0;
        /**
         * @brief Visits a boolean constant expression.
         * 
         * @param bool_const the boolean constant expression to be visited,
         * not null
         */
        virtual void VisitBoolConstant(BoolConstant* bool_const) = 0;
        /**
         * @brief Visits a string constant expression.
         * 
         * @param string_const The string constant expression to be visited,
         * not null
         */
        virtual void VisitStringConstant(StringConstant* string_const) = 0;
        /**
         * @brief Visits a symbol expression.
         * 
         * @param sym the symbol expression to be visited, not null
         */
        virtual void VisitSymbol(Symbol* sym) = 0;
        /**
         * @brief Visits a table make elements expression.
         * 
         * @param table_make_elems the table make elements expression to be
         * visited, not null
         */
        virtual void VisitTableMakeElems(TableMakeElems* table_make_elems) = 0;
        /**
         * @brief Visits a table make pairs expression.
         * 
         * @param table_make_pairs the table make pairs expression to be
         * visited, not null
         */
        virtual void VisitTableMakePairs(TableMakePairs* table_make_pairs) = 0;
    };
    }
}

#endif