%{
    #include <stdio.h>
    #include <iostream>
    #include <fstream>
    #include <list>
    #include <stack>
    #include <string>
    #include "../include/expression/primary.h"
    #include "../include/expression/arithmetic_expr.h"
    #include "../include/expression/bool_expr.h"
    #include "../include/expression/assign_expr.h"
    #include "../include/expression/symbol.h"
    #include "../include/expression/bool_constant.h"
    #include "../include/expression/constant.h"
    #include "../include/expression/nil_constant.h"
    #include "../include/expression/string_constant.h"
    #include "../include/expression/double_constant.h"
    #include "../include/expression/int_constant.h"
    #include "../include/expression/numeric_constant.h"
    #include "../include/expression/elist.h"
    #include "../include/expression/call.h"
    #include "../include/expression/call_suffix.h"
    #include "../include/expression/norm_call.h"
    #include "../include/expression/method_call.h"
    #include "../include/expression/tablemake.h"
    #include "../include/expression/tablemake_elems.h"
    #include "../include/expression/indexed_elem.h"
    #include "../include/expression/indexed.h"
    #include "../include/expression/tablemake_pairs.h"
    #include "../include/symbol_table.h"
    #include "../include/program_stack.h"
    #include "../include/instruction_opcodes.h"
    #include "../include/quad.h"
    #include "../include/stmt.h"
    #include "../include/for_stmt.h"
    #include "../include/while_stmt.h"
    #include "../include/loop_stmt.h"
    #include "../include/func_def_stmt.h"
    #include "../include/if_stmt.h"

    #ifdef DELOG
        #define DLOG(message) std::cout << message << std::endl
    #else
        #define DLOG(message)
    #endif
    
    int yyerror(std::string yaccProvidedMessage);
    int yylex(void);

    extern int yylineno;
    extern char* yytext;
    extern FILE* yyin;

    #define OUT_OF_SCOPE        -1
    #define LIB_FUNC_LINE       0
    #define TEMP_LINE           0
    #define BOOL_EXPR_CAST(e)   static_cast<BoolExpr*>(e)

    const unsigned int          global_scope = 0;
    unsigned int                current_scope = OUT_OF_SCOPE;
    
    SymbolTable                 symbol_table;
    ProgramStack                program_stack;  
    std::vector<Quad*>          quads;

    unsigned int                program_var_offset = 0;
    
    std::stack<LoopStmt*>       loop_stmts;
    std::stack<WhileStmt*>      while_stmts;
    std::stack<ForStmt*>        for_stmts;
    std::stack<FuncDefStmt*>    func_def_stmts;  
    std::stack<IfStmt*>         if_stmts;

    std::list<StmtType>         stmt_stack;

    bool                        NoErrorSignaled();
    void                        SignalError(std::string msg);
    void                        LogWarning(std::string msg);
    void                        LogQuads(std::ostream& output);
    void                        LogSymTable(std::ostream& output);
                           
    void                        InitLibraryFunctions();

    void                        IncreaseScope();
    void                        DecreaseScope();
    void                        HideLowerScopes();

    void                        DefineSymbol(Symbol* symbol);
    Symbol*                     DefineNewSymbol(ExprType type, const char* symbol, Expression* index);
    Symbol*                     DefineNewAnonymousFunc();
    void                        StashFormalArgument(Symbol* symbol);
    void                        DefineStashedFormalArguments();

    Symbol*                     NewTemp(ExprType type, Expression* index);
    void                        ResetTemp();
    Quad*                       Emit(Iopcode op, Expression* result, Expression* arg1, Expression* arg2);

    Symbol*                     EmitIfTableItem(Symbol* sym);
    Symbol*                     MemberItem(Symbol* sym, const char* id);
    Call*                       MakeCall(Symbol* called_symbol, CallSuffix* call_suffix);

    unsigned int                NextQuadLabel();

    bool                        IsLibraryFunction(Expression* expr);
    bool                        IsUserFunction(Expression* expr);
    bool                        IsVariable(Expression* expr);
    bool                        IsConstString(Expression* expr);
    bool                        IsConstBool(Expression* expr);
    bool                        IsTableMake(Expression* expr);

    bool                        IsGlobalVar(Symbol* symbol);
    bool                        IsAtCurrentScope(Symbol* symbol);
    bool                        IsTableItem(Symbol* symbol);
    bool                        IsMethodCall(CallSuffix* call_suffix);

    bool                        InLoop();
    bool                        InFuncDef();

    void                        BackPatch(std::list<unsigned int> l_list, unsigned int q_label);
    Symbol*                     ConcludeShortCircuit(BoolExpr* expr);

    bool                        IsValidArithmeticOp(Expression* expr);
    bool                        IsValidArithmeticComp(Expression* expr);
    bool                        IsValidAssign(Symbol* left_operand);
    bool                        IsValidBreakContinue();
%}

%union {                                                    
    char*                       stringValue;
    int                         intValue;
    double                      doubleValue;

    unsigned int                quad_label;

    class FuncDefStmt*          funcdef;

    class Expression*           expr;
    class AssignExpr*           assignexpr;

    class Primary*              prim;
    class Constant*             con;
    class Call*                 call;
    class CallSuffix*           call_suffix;
    class NormCall*             norm_call;
    class MethodCall*           method_call;
    class Elist*                elist;
    class Symbol*               sym;
    class TableMake*            tablemake;
    class Indexed*              indexed;
    class IndexedElem*          indexed_elem; 
}

%start program

%token EQUAL NOTEQUAL COLONCOLON DOTDOT GEQL LEQL MINUSMINUS PLUSPLUS
%token LOCAL FUNCTION IF ELSE FOR WHILE BREAK CONTINUE NIL TRUE FALSE RETURN

%token <stringValue>    STRING ID 
%token <intValue>       INTNUM
%token <doubleValue>    DOUBLENUM

%type <quad_label>    M

%type <funcdef>         funcdef 

%type <expr>            term expr

%type <assignexpr>      assignexpr

%type <prim>            primary
%type <con>             const
%type <sym>             lvalue member
%type <call>            call
%type <call_suffix>     callsuffix
%type <norm_call>       normcall
%type <method_call>     methodcall
%type <elist>           elist multelist
%type <tablemake>       objectdef
%type <indexed>         indexed multindexed
%type <indexed_elem>    indexedelem

%right      '='
%left       OR
%left       AND
%nonassoc   EQUAL NOTEQUAL
%nonassoc   '>' GEQL '<' LEQL
%left       '-' '+'
%left       '*' '/' '%'
%nonassoc   UMINUS
%right      NOT MINUSMINUS PLUSPLUS 
%left       '.' DOTDOT
%left       '[' ']'
%left       '(' ')'

%%

program:      stmts                 {
                                        DLOG("program -> stmts");
                                    }
            ;

stmts:      stmt                    {
                                        //ResetTemp();
                                    }
            stmts                   {
                                        DLOG("stmts -> stmt stmts");
                                    }
            |                       {
                                        DLOG("stmts -> EMPTY");
                                    }
            ;

stmt:         expr ';'              {
                                        if ($1->get_type() == BOOL) {
                                            ConcludeShortCircuit(BOOL_EXPR_CAST($1));
                                        }
                                        ResetTemp();
                                        DLOG("stmt -> expr;");
                                    }
            | ifstmt                {
                                        DLOG("stmt -> ifstmt");
                                    }
            | whilestmt             {
                                        DLOG("stmt -> whilestmt");
                                    }
            | forstmt               {
                                        DLOG("stmt -> forstmt");
                                    }
            | returnstmt            {
                                        DLOG("stmt -> returnstmt");
                                    }
            | BREAK ';'             { 
                                        if (!IsValidBreakContinue()) {
                                            SignalError("invalid keyword BREAK outside of loop");
                                        } else {
                                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);

                                            auto top_loop_stmt = loop_stmts.top();
                                            top_loop_stmt->PushBreakJumpQuad(jump_quad);
                                        }

                                        DLOG("stmt -> break;");
                                    }
            | CONTINUE ';'          {
                                        if (!IsValidBreakContinue()) {
                                            SignalError("invalid keyword CONTINUE outside of loop");
                                        } else {
                                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);

                                            auto top_loop_stmt = loop_stmts.top();
                                            top_loop_stmt->PushContinueJumpQuad(jump_quad);
                                        }
                                        
                                        DLOG("stmt -> continue;");
                                    }
            | block                 {
                                        DLOG("stmt -> block");
                                    }
            | funcdef               {
                                        DLOG("stmt -> funcdef");
                                    }
            | ';'                   {
                                        DLOG("stmt -> ;");
                                    }
            ;

expr:         assignexpr            {
                                        $$ = $1;
                                        DLOG("expr -> assignexpr");
                                    }
            | expr '+' expr         {
                                        auto expr1 = $1;
                                        auto expr2 = $3;
                                        if (IsValidArithmeticOp(expr1) & IsValidArithmeticOp(expr2)) {
                                            auto temp = NewTemp(VAR, nullptr);
                                            Emit(ADD_t, temp, expr1, expr2);
                                            $$ = new ArithmeticExpr(temp, expr1, expr2);
                                        }
                                        DLOG("expr -> expr + expr");
                                    }
            | expr '-' expr         {
                                        auto expr1 = $1;
                                        auto expr2 = $3;
                                        if (IsValidArithmeticOp(expr1) & IsValidArithmeticOp(expr2)) {
                                            auto temp = NewTemp(VAR, nullptr);
                                            Emit(SUB_t, temp, expr1, expr2);
                                            $$ = new ArithmeticExpr(temp, expr1, expr2);
                                        } 
                                        DLOG("expr -> expr - expr");
                                    }
            | expr '*' expr         {
                                        auto expr1 = $1;
                                        auto expr2 = $3;
                                        if (IsValidArithmeticOp(expr1) & IsValidArithmeticOp(expr2)) {
                                            auto temp = NewTemp(VAR, nullptr);
                                            Emit(MUL_t, temp, expr1, expr2);
                                            $$ = new ArithmeticExpr(temp, expr1, expr2);
                                        }
                                        DLOG("expr -> expr * expr");
                                    }
            | expr '/' expr         {
                                        auto expr1 = $1;
                                        auto expr2 = $3;
                                        if (IsValidArithmeticOp(expr1) & IsValidArithmeticOp(expr2)) {
                                            auto temp = NewTemp(VAR, nullptr);
                                            Emit(DIV_t, temp, expr1, expr2);
                                            $$ = new ArithmeticExpr(temp, expr1, expr2);
                                        }
                                        DLOG("expr -> expr / expr");
                                    }
            | expr '%' expr         {
                                        auto expr1 = $1;
                                        auto expr2 = $3;
                                        if (IsValidArithmeticOp(expr1) & IsValidArithmeticOp(expr2)) {
                                            auto temp = NewTemp(VAR, nullptr);
                                            Emit(MOD_t, temp, expr1, expr2);
                                            $$ = new ArithmeticExpr(temp, expr1, expr2);
                                        } 
                                        DLOG("expr -> expr % expr");
                                    }
            | expr '>' expr         {
                                        auto expr1 = $1;
                                        auto expr2 = $3;
                                        if (IsValidArithmeticComp(expr1) & IsValidArithmeticComp(expr2)) {
                                            BoolExpr* n_expr = new BoolExpr(expr1, expr2, nullptr);

                                            auto greater_quad = Emit(IF_GREATER_t, expr1, expr2, nullptr);

                                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);

                                            n_expr->true_list.push_back(greater_quad->label);
                                            n_expr->false_list.push_back(jump_quad->label);

                                            $$ = n_expr;
                                        }
                                        DLOG("expr -> expr > expr");
                                    }
            | expr GEQL expr        {
                                        auto expr1 = $1;
                                        auto expr2 = $3;
                                        if (IsValidArithmeticComp(expr1) & IsValidArithmeticComp(expr2)) {
                                            BoolExpr* n_expr = new BoolExpr(expr1, expr2, nullptr);

                                            auto greater_equal_quad = Emit(IF_GREATEREQ_t, expr1, expr2, nullptr);

                                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);

                                            n_expr->true_list.push_back(greater_equal_quad->label);
                                            n_expr->false_list.push_back(jump_quad->label);

                                            $$ = n_expr;
                                        }

                                        DLOG("expr -> expr >= expr");
                                    }
            | expr '<' expr         {
                                        auto expr1 = $1;
                                        auto expr2 = $3;
                                        if (IsValidArithmeticComp(expr1) & IsValidArithmeticComp(expr2)) {
                                            BoolExpr* n_expr = new BoolExpr(expr1, expr2, nullptr);

                                            auto less_quad = Emit(IF_LESS_t, expr1, expr2, nullptr);

                                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);

                                            n_expr->true_list.push_back(less_quad->label);
                                            n_expr->false_list.push_back(jump_quad->label);

                                            $$ = n_expr;
                                        }
                                        DLOG("expr -> expr + expr");
                                    }
            | expr LEQL expr        {
                                        auto expr1 = $1;
                                        auto expr2 = $3;
                                        if (IsValidArithmeticComp(expr1) & IsValidArithmeticComp(expr2)) {
                                            BoolExpr* n_expr = new BoolExpr(expr1, expr2, nullptr);

                                            auto less_equal_quad = Emit(IF_LESSEQ_t, expr1, expr2, nullptr);

                                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);

                                            n_expr->true_list.push_back(less_equal_quad->label);
                                            n_expr->false_list.push_back(jump_quad->label);

                                            $$ = n_expr;
                                        }
                                        DLOG("expr -> expr <= expr");
                                    }
            | expr EQUAL            {
                                        if ($1->get_type() == BOOL) {
                                            $1 = ConcludeShortCircuit(BOOL_EXPR_CAST($1));
                                        }
                                    }
            expr                    {
                                        if ($4->get_type() == BOOL) {
                                            $4 = ConcludeShortCircuit(BOOL_EXPR_CAST($4));
                                        }

                                        auto expr1 = $1;
                                        auto expr2 = $4;

                                        BoolExpr* n_expr = new BoolExpr(expr1, expr2, nullptr);

                                        auto equal_quad = Emit(IF_EQ_t, expr1, expr2, nullptr);

                                        auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);

                                        n_expr->true_list.push_back(equal_quad->label);
                                        n_expr->false_list.push_back(jump_quad->label);

                                        $$ = n_expr;

                                        DLOG("expr -> expr == expr");
                                    }
            | expr NOTEQUAL         {
                                        if ($1->get_type() == BOOL) {
                                            $1 = ConcludeShortCircuit(BOOL_EXPR_CAST($1));
                                        }
                                    }
            expr                    {
                                        if ($4->get_type() == BOOL) {
                                            $4 = ConcludeShortCircuit(BOOL_EXPR_CAST($4));
                                        }

                                        auto expr1 = $1;
                                        auto expr2 = $4;

                                        BoolExpr* n_expr = new BoolExpr(expr1, expr2, nullptr);

                                        auto not_equal_quad = Emit(IF_NOTEQ_t,  expr1, expr2, nullptr);

                                        auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);

                                        n_expr->true_list.push_back(not_equal_quad->label);
                                        n_expr->false_list.push_back(jump_quad->label);

                                        $$ = n_expr;

                                        DLOG("expr -> expr != expr");
                                    }
            | expr AND              {
                                        auto tmp_expr1 = $1;
                                        if (tmp_expr1->get_type() != BOOL) {
                                            BoolExpr* expr1 = new BoolExpr(tmp_expr1, nullptr, nullptr);

                                            auto equal_quad = Emit(IF_EQ_t, tmp_expr1, new BoolConstant(true), nullptr);
                                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);

                                            expr1->true_list.push_back(equal_quad->label);
                                            expr1->false_list.push_back(jump_quad->label);

                                            $1 = expr1;
                                        }
                                    }
            M expr                  {
                                        auto expr1 = $1;
                                        auto expr2 = $5;
                                        auto q_label = $4;

                                        if (expr2->get_type() != BOOL) {
                                            expr2 = new BoolExpr($5, nullptr, nullptr);

                                            auto equal_quad = Emit(IF_EQ_t, $5, new BoolConstant(true), nullptr);
                                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);

                                            BOOL_EXPR_CAST(expr2)->true_list.push_back(equal_quad->label);
                                            BOOL_EXPR_CAST(expr2)->false_list.push_back(jump_quad->label);
                                        }

                                        BoolExpr* n_expr = new BoolExpr(expr1, expr2, nullptr);

                                        BackPatch(BOOL_EXPR_CAST(expr1)->true_list, q_label);
                                        n_expr->true_list = BOOL_EXPR_CAST(expr2)->true_list;
                                        n_expr->false_list = BOOL_EXPR_CAST(expr1)->false_list;
                                        n_expr->false_list.merge(BOOL_EXPR_CAST(expr2)->false_list);

                                        $$ = n_expr;

                                        DLOG("expr -> expr and expr");
                                    }
            | expr OR               {
                                        auto tmp_expr1 = $1;
                                        if (tmp_expr1->get_type() != BOOL) {
                                            BoolExpr* expr1 = new BoolExpr(tmp_expr1, nullptr, nullptr);

                                            auto equal_quad = Emit(IF_EQ_t, tmp_expr1, new BoolConstant(true), nullptr);
                                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);

                                            expr1->true_list.push_back(equal_quad->label);
                                            expr1->false_list.push_back(jump_quad->label);

                                            $1 = expr1;
                                        }
                                    }
            M expr                  {
                                        auto expr1 = $1;
                                        auto expr2 = $5;
                                        auto q_label = $4;

                                        if (expr2->get_type() != BOOL) {
                                            expr2 = new BoolExpr($5, nullptr, nullptr);

                                            auto equal_quad = Emit(IF_EQ_t, $5, new BoolConstant(true), nullptr);
                                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);

                                            BOOL_EXPR_CAST(expr2)->true_list.push_back(equal_quad->label);
                                            BOOL_EXPR_CAST(expr2)->false_list.push_back(jump_quad->label);
                                        }

                                        BoolExpr* n_expr = new BoolExpr(expr1, expr2, nullptr);

                                        BackPatch(BOOL_EXPR_CAST(expr1)->false_list, q_label);
                                        n_expr->true_list = BOOL_EXPR_CAST(expr1)->true_list;
                                        n_expr->true_list.merge(BOOL_EXPR_CAST(expr2)->true_list);
                                        n_expr->false_list = BOOL_EXPR_CAST(expr2)->false_list;

                                        $$ = n_expr;

                                        DLOG("expr -> assignexpr");
                                    }
            | term                  {
                                        $$ = $1;
                                        DLOG("expr -> term");
                                    }
            ;

M:                                  {
                                        $<quad_label>$ = NextQuadLabel();
                                    }
            ;

term:         '(' expr ')'          {
                                        $$ = $2;
                                        DLOG("term -> (expr)");
                                    }
            | '-' expr %prec UMINUS {
                                        auto symbol = $2;
                                        if (IsValidArithmeticOp(symbol)) {
                                            auto temp = NewTemp(VAR, nullptr);
                                            Emit(UMINUS_t, temp, symbol, nullptr);
                                            $$ = symbol;
                                        }
                                        DLOG("term -> -expr");
                                    }
            | NOT expr              {
                                        auto expr1 = $2;

                                        if (expr1->get_type() != BOOL) {
                                            expr1 = new BoolExpr($2, nullptr, nullptr);
                                        }

                                        BoolExpr* n_expr = new BoolExpr(expr1, nullptr, nullptr);

                                        auto equal_quad = Emit(IF_EQ_t, expr1, new BoolConstant(true),  nullptr);

                                        auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);
                                        
                                        BOOL_EXPR_CAST(expr1)->true_list.push_back(equal_quad->label);
                                        BOOL_EXPR_CAST(expr1)->false_list.push_back(jump_quad->label);

                                        n_expr->true_list = BOOL_EXPR_CAST(expr1)->false_list;
                                        n_expr->false_list = BOOL_EXPR_CAST(expr1)->true_list;

                                        $$ = n_expr;

                                        DLOG("term -> not expr");
                                    }
            | PLUSPLUS lvalue       {
                                        auto symbol = $2;
                                        Symbol* result;
                                        if (IsValidArithmeticOp(symbol)) {
                                            if (IsTableItem(symbol)) {
                                                result = EmitIfTableItem(symbol);
                                                Emit(ADD_t, result, result, new IntConstant(1));
                                                Emit(TABLESETELEM_t, symbol, symbol->get_index(), result);
                                            } else {
                                                result = NewTemp(VAR, nullptr);
                                                Emit(ADD_t, symbol, symbol, new IntConstant(1));
                                                Emit(ASSIGN_t, result, symbol, nullptr);  
                                            }
                                        } 
                                        $$ = result;     
                                        DLOG("term -> ++lvalue"); 
                                    }
            | lvalue PLUSPLUS       {
                                        auto symbol = $1;
                                        auto result = NewTemp(VAR, nullptr);
                                        if (IsValidArithmeticOp(symbol)) {
                                            if (IsTableItem(symbol)) {
                                                auto val = EmitIfTableItem(symbol);
                                                Emit(ASSIGN_t, result, val, nullptr);
                                                Emit(ADD_t, val, val, new IntConstant(1));
                                                Emit(TABLESETELEM_t, symbol, symbol->get_index(), val);
                                            } else {
                                                Emit(ASSIGN_t, result, symbol, nullptr);    
                                                Emit(ADD_t, symbol, symbol, new IntConstant(1));
                                            }
                                        } 
                                        $$ = result;
                                        DLOG("term -> lvalue++"); }
            | MINUSMINUS lvalue     { 
                                        auto symbol = $2;
                                        Symbol* result;
                                        if (IsValidArithmeticOp(symbol)) {
                                            if (IsTableItem(symbol)) {
                                                result = EmitIfTableItem(symbol);
                                                Emit(SUB_t, result, result, new IntConstant(1));
                                                Emit(TABLESETELEM_t, symbol, symbol->get_index(), result);
                                            } else {
                                                result = NewTemp(VAR, nullptr);
                                                Emit(SUB_t, symbol, symbol, new IntConstant(1));
                                                Emit(ASSIGN_t, result, symbol, nullptr);  
                                            }
                                        } 
                                        $$ = result;      
                                        DLOG("term -> --lvaule");
                                    }
            | lvalue MINUSMINUS     { 
                                        auto symbol = $1;
                                        auto result = NewTemp(VAR, nullptr);
                                        if (IsValidArithmeticOp(symbol)) {
                                            if (IsTableItem(symbol)) {
                                                auto val = EmitIfTableItem(symbol);
                                                Emit(ASSIGN_t, result, val, nullptr);
                                                Emit(SUB_t, val, val, new IntConstant(1));
                                                Emit(TABLESETELEM_t, symbol, symbol->get_index(), val);
                                            } else {
                                                Emit(ASSIGN_t, result, symbol, nullptr);    
                                                Emit(SUB_t, symbol, symbol, new IntConstant(1));
                                            }
                                        } 
                                        $$ = result;  
                                        DLOG("term -> lvalue--");
                                    }
            | primary               {   
                                        $$ = $1;
                                        DLOG("term -> primary"); 
                                    }
            ;

assignexpr:   lvalue '=' expr       {
                                        auto symbol = $1;
                                        auto expr = $3;
                                        if ($3->get_type() == BOOL) {
                                            expr = ConcludeShortCircuit(BOOL_EXPR_CAST($3));
                                        }
                                        if (IsValidAssign(symbol)) {
                                            if (IsTableItem(symbol)) {
                                                Emit(TABLESETELEM_t, symbol, symbol->get_index(), expr);
                                                auto result = EmitIfTableItem(symbol);
                                                $$ = new AssignExpr(result, symbol, expr);
                                            } else {
                                                auto result = NewTemp(VAR, nullptr);
                                                Emit(ASSIGN_t, symbol, expr, nullptr);
                                                Emit(ASSIGN_t, result, symbol, nullptr);
                                                $$ = new AssignExpr(result, symbol, expr);
                                            }
                                        }
                                        DLOG("assignexpr -> lvalue = expr");
                                    }
            ;

primary:      lvalue                {
                                        $$ = EmitIfTableItem($1);

                                        DLOG("primary -> lvalue");
                                    }
            | call                  {
                                        $$ = $1;
                                        DLOG("primary -> call");
                                    }
            | objectdef             {
                                        $$ = $1;
                                        DLOG("primary -> objectdef");
                                    }
            | '(' funcdef ')'       {   
                                        auto func_def = $2;
                                        auto func = func_def->get_sym();
                                        $$ = func;
                                        DLOG("primary -> (funcdef)");
                                    }
            | const                 {
                                        $$ = $1;
                                        DLOG("primary -> const");
                                    }
            ;

lvalue:       ID                    {
                                        auto symbol = program_stack.Lookup($1);
                                        if (symbol == nullptr) {
                                            symbol = DefineNewSymbol(VAR, $1, nullptr);
                                        } else if (!symbol->is_active()) {
                                            SignalError("Cannot access " + symbol->get_id() + ", previously defined in line: " + std::to_string(symbol->get_line()));
                                        }
                                        $$ = symbol;

                                        DLOG("lvalue -> id");
                                    }
            | LOCAL ID              {
                                        auto symbol = program_stack.Lookup($2);
                                        if (symbol == nullptr) { 
                                            symbol = DefineNewSymbol(VAR, $2, nullptr);
                                        }
                                        else if (!symbol->is_active()) {
                                            SignalError("Cannot access " + symbol->get_id() + ", previously defined in line: " + std::to_string(symbol->get_line()));
                                        }
                                        else if (IsGlobalVar(symbol)) {
                                            symbol = DefineNewSymbol(VAR, $2, nullptr);
                                        }
                                        else if (IsUserFunction(symbol)){
                                            if(IsAtCurrentScope(symbol)) 
                                                SignalError("Attempting to redefine a previously declared user function");
                                            else 
                                                symbol = DefineNewSymbol(VAR, $2, nullptr);
                                        }
                                        else if (IsLibraryFunction(symbol)) {
                                            SignalError("Attempting to redefine a library function");
                                        }
                                        $$ = symbol;
                                        DLOG("lvalue -> local id");
                                    }
            | COLONCOLON ID         {
                                        auto symbol = program_stack.LookupGlobal($2);
                                        if (symbol == nullptr || !symbol->is_active()) 
                                            SignalError("No global variable with id: " + std::string($2));
                                        $$ = symbol;
                                        DLOG("lvalue -> ::id");
                                    }
            | member                {
                                        $$ = $1;
                                        DLOG("lvalue -> member");
                                    }
            ;

member:     lvalue '.' ID           {
                                        auto item = $1;
                                        auto index = $3;
                                        $$ = MemberItem(item, index);
                                        DLOG("member -> lvalue.id");
                                    }
            | lvalue '[' expr ']'   {
                                        auto item = $1;
                                        auto index = $3;
                                        auto sym = EmitIfTableItem(item);
                                        $$ = DefineNewSymbol(TABLE_ITEM, sym->get_id().c_str(), index);
                                        DLOG("member -> lvalue[expr]");
                                    }
            | call '.' ID           {
                                        auto item = ($1)->get_ret_val();
                                        auto index = $3;
                                        $$ = MemberItem(item, index);
                                        DLOG("member -> call.id");
                                    }
            | call '[' expr ']'     {
                                        auto item = ($1)->get_ret_val();
                                        auto index = $3;
                                        auto sym = EmitIfTableItem(item);
                                        $$ = DefineNewSymbol(TABLE_ITEM, sym->get_id().c_str(), index);
                                        DLOG("member -> call[expr]");
                                    }
            ;

call:       call  '(' elist ')'             {
                                                auto called_symbol = $1->get_called_symbol();
                                                $$ = MakeCall(called_symbol, new NormCall($3));
                                                DLOG("call -> call(elist)");
                                            }
            | lvalue  callsuffix            {
                                                auto called_symbol = $1;
                                                auto call_suffix = $2;
                                                called_symbol = EmitIfTableItem(called_symbol);
                                                if (IsMethodCall(call_suffix)) {
                                                    auto t = called_symbol;
                                                    called_symbol = EmitIfTableItem(MemberItem(t, call_suffix->get_name().c_str()));
                                                    call_suffix->InsertArg(t);
                                                }
                                                $$ = MakeCall(called_symbol, call_suffix);
                                            }
            | '(' funcdef ')'  
                '(' elist ')'               {
                                                auto called_symbol = $2->get_sym();
                                                $$ = MakeCall(called_symbol, new NormCall($5)); 
                                                DLOG("call -> (funcdef)(elist)");
                                            }
            ;

callsuffix: normcall        {
                                $$ = $1;
                                DLOG("callsuffix -> normcall");
                            }
            | methodcall    {
                                $$ = $1;
                                DLOG("callsuffix -> methodcall");
                            }
            ;

normcall:   '(' elist ')'   {
                                $$ = new NormCall($2);
                                DLOG("normcall -> (elist)"); 
                            }
            ;

methodcall: DOTDOT ID '(' elist ')' {
                                        $$ = new MethodCall(std::string($2), $4);
                                        DLOG("methodcall -> ..id(elist)");
                                    }
            ;

multelist:  ',' expr multelist  {
                                    if ($2->get_type() == BOOL) {
                                        $2 = ConcludeShortCircuit(BOOL_EXPR_CAST($2));
                                    }
                                    Elist* elist = new Elist();
                                    elist->exprs.merge($3->exprs);
                                    elist->exprs.push_back($2);
                                    $$ = elist;
                                    DLOG("multelist -> ,expr multelist");
                                }
            |                   {
                                    $$ = new Elist();
                                    DLOG("multelist -> EMPTY");
                                }
            ;

elist:      expr multelist  {
                                if ($1->get_type() == BOOL) {
                                    $1 = ConcludeShortCircuit(BOOL_EXPR_CAST($1));
                                }
                                Elist* elist = new Elist();
                                elist->exprs.merge($2->exprs);
                                elist->exprs.push_back($1);
                                $$ = elist; 
                                DLOG("elist -> expr multelist");
                            }
            |               {
                                $$ = new Elist();
                                DLOG("elist -> EMPTY");
                            }
            ;

objectdef:  '[' elist ']'       {
                                    auto result = NewTemp(VAR, nullptr);
                                    Emit(TABLECREATE_t, result, nullptr, nullptr);

                                    auto elements = $2->exprs;
                                    unsigned int elem_cnt = 0;
                                    elements.reverse();
                                    for (auto element : elements)
                                        Emit(TABLESETELEM_t, result, new IntConstant(elem_cnt++), element);
                                    $$ = new TableMakeElems(result, $2);    
                                    DLOG("objectdef -> [elist]");
                                }
            | '[' indexed ']'   {
                                    auto result = NewTemp(VAR, nullptr);
                                    Emit(TABLECREATE_t, result, nullptr, nullptr);
                                    auto indexedelems = $2->pairs;
                                    indexedelems.reverse();
                                    for (auto indexedelem : indexedelems) 
                                        Emit(TABLESETELEM_t, result, indexedelem->pair.first, indexedelem->pair.second);
                                    $$ = new TableMakePairs(result, $2);    
                                    DLOG("objectdef -> [indexed]");
                                }
            ;

multindexed:',' indexedelem multindexed {
                                            Indexed* indexed = new Indexed();
                                            indexed->pairs.merge($3->pairs);
                                            indexed->pairs.push_back($2);
                                            $$ = indexed;
                                            DLOG("multindexed -> , indexedelem multidexed"); 
                                        }
            |                           {
                                            $$ = new Indexed();
                                            DLOG("elsestmt -> EMPTY");
                                        }
            ;

indexed:    indexedelem multindexed {
                                        Indexed* indexed = new Indexed();
                                        indexed->pairs.merge($2->pairs);
                                        indexed->pairs.push_back($1);
                                        $$ = indexed; 
                                        DLOG("indexed -> indexedelem multidexed"); 
                                    }
            ;

indexedelem:'{' expr ':' expr '}'   {
                                        if ($4->get_type() == BOOL) {
                                            $4 = ConcludeShortCircuit(BOOL_EXPR_CAST($4));
                                        }
                                        $$ = new IndexedElem(std::pair<Expression*, Expression*>($2, $4));
                                    }
            ;

block:      '{'         {
                            IncreaseScope();
                            DefineStashedFormalArguments();
                        }
            stmts '}'   {
                            DecreaseScope();
                            DLOG("block -> { stmts }");
                        }
            ;

funcdef:    FUNCTION        {
                                auto anonymous_function = DefineNewAnonymousFunc();
                                auto func_def_stmt = new FuncDefStmt(anonymous_function);

                                func_def_stmts.push(func_def_stmt);

                                auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);
                                Emit(FUNCSTART_t, anonymous_function, nullptr, nullptr);

                                func_def_stmt->set_func_start_jump_quad(jump_quad);

                                $<funcdef>$ = func_def_stmt;
                            }
            '(' idlist ')'  {
                                HideLowerScopes();
                                stmt_stack.push_back(FUNC_t);
                            }
            block           {
                                auto top_func_def = func_def_stmts.top();
                                auto anonymous_function = top_func_def->get_sym();

                                auto func_end_quad = Emit(FUNCEND_t, anonymous_function, nullptr, nullptr);

                                top_func_def->PatchFuncStartJumpQuad(func_end_quad->label + 1);
                                top_func_def->PatchReturnJumpQuads(func_end_quad->label);

                                program_stack.ActivateLowerScopes();

                                func_def_stmts.pop();

                                stmt_stack.pop_back();

                                $<funcdef>$ = top_func_def;
                                DLOG("funcdef -> function (idlist) block "); 
                            }
            | FUNCTION ID   {
                                auto symbol = program_stack.Lookup($2);
                                if (symbol == nullptr) {
                                    symbol = DefineNewSymbol(USER_FUNC, $2, nullptr);
                                }
                                else if (!symbol->is_active()) {
                                    SignalError("Cannot access " + symbol->get_id() + ", previously defined in line: " + std::to_string(symbol->get_line()));
                                }
                                else {
                                    if (IsVariable(symbol)) {
                                        SignalError(std::string($2) + " variable, previously defined in line: " + std::to_string(symbol->get_line()) + ", cannot be redefined as a function");
                                    }
                                    else if (IsLibraryFunction(symbol)) {
                                        SignalError(std::string($2) + " library function cannot be shadowed by a user function");
                                    }
                                    else if (IsAtCurrentScope(symbol)) {
                                        SignalError("Name collision with function " + std::string($2) + ", previously defined in line: " + std::to_string(symbol->get_line()));
                                    }
                                    else{
                                        symbol = DefineNewSymbol(USER_FUNC, $2, nullptr);  // shadow user function
                                    }
                                }
                                auto func_def_stmt = new FuncDefStmt(symbol); 
                                func_def_stmts.push(func_def_stmt);

                                auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);
                                Emit(FUNCSTART_t, symbol, nullptr, nullptr);

                                func_def_stmt->set_func_start_jump_quad(jump_quad);

                                $<funcdef>$ = func_def_stmt;
                            }
            '(' idlist ')'  {
                                HideLowerScopes();
                                stmt_stack.push_back(FUNC_t);
                            }
            block           { 
                                auto top_func_def =  func_def_stmts.top();
                                auto function = top_func_def->get_sym();
                                auto func_end_quad = Emit(FUNCEND_t, function, nullptr, nullptr);

                                top_func_def->PatchFuncStartJumpQuad(func_end_quad->label + 1);
                                top_func_def->PatchReturnJumpQuads(func_end_quad->label);

                                func_def_stmts.pop();

                                program_stack.ActivateLowerScopes();

                                stmt_stack.pop_back();

                                $<funcdef>$ = top_func_def;
                                DLOG("funcdef -> function id (idlist) block"); 
                            }
            ;

const:        INTNUM    {
                            $$ = new IntConstant($1);
                            DLOG("const -> INTNUM");
                        }
            | DOUBLENUM { 
                            $$ = new DoubleConstant($1); 
                            DLOG("const -> DOUBLENUM"); 
                        }
            | STRING    { 
                            $$ = new StringConstant(std::string($1));
                            DLOG("const -> STRING"); 
                        }
            | NIL       { 
                            $$ = new NilConstant(nullptr);
                            DLOG("const -> NIL"); 
                        }
            | TRUE      { 
                            $$ = new BoolConstant(true);
                            DLOG("const -> TRUE"); 
                        }
            | FALSE     {   
                            $$ = new BoolConstant(false);
                            DLOG("const -> FALSE");
                        }
            ;

multid:     ',' ID  {
                        auto top_func_def_stmt = func_def_stmts.top();
                        auto func = top_func_def_stmt->get_sym();
                        auto offset = func->get_formal_arguments().size();

                        auto new_formal_arg = new Symbol(VAR, $2, yylineno, current_scope + 1, FORMAL_ARG, offset, nullptr);
                        if (func->HasFormalArg(new_formal_arg)) {
                            SignalError("formal argument " + std::string($2) + " already declared");
                        }
                        else {
                            func->AddFormalArg(new_formal_arg);
                            StashFormalArgument(new_formal_arg);       
                        }
                    } 
            multid  {
                        DLOG("multid -> , id multid");
                    }
            |       {
                        DLOG("multid -> EMPTY");
                    }
            ;

idlist:     ID      {
                        auto top_func_def_stmt = func_def_stmts.top();
                        auto func = top_func_def_stmt->get_sym();
                        auto offset = func->get_formal_arguments().size();

                        auto new_formal_arg = new Symbol(VAR, $1, yylineno, current_scope + 1, FORMAL_ARG, offset, nullptr);
                        if (func->HasFormalArg(new_formal_arg)) {
                            SignalError("formal argument " + std::string($1) + " already declared");
                        }
                        else {
                            func->AddFormalArg(new_formal_arg);   
                            StashFormalArgument(new_formal_arg);  
                        }
                    } 
            multid  { 
                        DLOG("idlist -> id multid"); 
                    }
            |       {
                        DLOG("idlist -> EMPTY");
                    }
            ;

ifstmt:     IF '(' expr ')'                 {
                                                if ($3->get_type() == BOOL) {
                                                    $3 = ConcludeShortCircuit(BOOL_EXPR_CAST($3));
                                                }

                                                auto if_stmt = new IfStmt();
                                                if_stmts.push(if_stmt);

                                                auto branch_quad = Emit(IF_EQ_t, $3, new BoolConstant(true), nullptr);
                                                auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr); 

                                                PatchBranchQuad(branch_quad, branch_quad->label + 2);

                                                if_stmt->set_if_jump_quad(jump_quad);
                                            }
            stmt                            {
                                                //ResetTemp();
                                            }
            elsestmt                        {
                                                DLOG("ifstmt -> if (expr) stmt elsestmt"); 
                                            }
            ;

elsestmt:   ELSE            {
                                auto top_if_stmt = if_stmts.top();

                                auto else_jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);

                                top_if_stmt->PushElseJumpQuad(else_jump_quad);
                                top_if_stmt->PatchIfJumpQuad(NextQuadLabel());
                            }
            stmt            {
                                auto top_if_stmt = if_stmts.top();

                                top_if_stmt->PatchElseJumpQuad(NextQuadLabel());
                                top_if_stmt->PopElseJumpQuad();

                                if_stmts.pop();

                                //ResetTemp();
                                DLOG("elsestmt -> else stmt"); 
                            }
            |               {
                                auto top_if_stmt = if_stmts.top();

                                top_if_stmt->PatchIfJumpQuad( NextQuadLabel());

                                if_stmts.pop();
                                DLOG("elsestmt -> EMPTY");
                            }
            ;

whilestmt:  WHILE               { 
                                    auto first_quad_label = NextQuadLabel();

                                    auto while_stmt = new WhileStmt(first_quad_label);

                                    while_stmts.push(while_stmt);
                                    loop_stmts.push(while_stmt);
                                }
            '(' expr ')'        {
                                    if ($4->get_type() == BOOL) {
                                        $4 = ConcludeShortCircuit(BOOL_EXPR_CAST($4));
                                    }

                                    auto top_while_stmt = while_stmts.top();

                                    auto branch_quad = Emit(IF_EQ_t, $4, new BoolConstant(true), nullptr);
                                    auto exit_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);

                                    top_while_stmt->PushLoopQuad(branch_quad);
                                    top_while_stmt->PushLoopQuad(exit_quad);

                                    stmt_stack.push_back(LOOP_t);
                                }
            stmt                { 
                                    auto top_while_stmt = while_stmts.top();

                                    auto loop_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);

                                    top_while_stmt->PushLoopQuad(loop_quad);
                                    top_while_stmt->PatchLoopQuads();
                                    top_while_stmt->PatchBreakJumpQuads(NextQuadLabel());
                                    top_while_stmt->PatchContinueJumpQuads();

                                    while_stmts.pop();
                                    loop_stmts.pop();

                                    stmt_stack.pop_back();

                                    //ResetTemp();
                                    DLOG ("whilestmt -> WHILE (expr) stmt"); 
                                }
            ;

forstmt:    FOR                                     {
                                                        auto first_quad_label = NextQuadLabel();

                                                        auto for_stmt = new ForStmt(first_quad_label);

                                                        for_stmts.push(for_stmt);
                                                        loop_stmts.push(for_stmt);
                                                    }                   
            '(' elist ';'                           {
                                                        auto top_for_stmt = for_stmts.top();

                                                        auto logical_expr_first_quad_label = NextQuadLabel();

                                                        top_for_stmt->set_logical_expr_first_quad_label(logical_expr_first_quad_label);
                                                    }
            expr ';'                                {
                                                        if ($7->get_type() == BOOL) {
                                                            $7 = ConcludeShortCircuit(BOOL_EXPR_CAST($7));
                                                        }

                                                        auto top_for_stmt = for_stmts.top();

                                                        auto branch_quad = Emit(IF_EQ_t, $7, new BoolConstant(true), nullptr);

                                                        top_for_stmt->PushLoopQuad(branch_quad);

                                                        auto exit_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);

                                                        top_for_stmt->PushLoopQuad(exit_quad);

                                                        auto exprs_first_quad_label = NextQuadLabel();

                                                        top_for_stmt->set_exprs_first_quad_label(exprs_first_quad_label);
                                                    }
            elist ')'                               {
                                                        auto top_for_stmt = for_stmts.top();
                                                        auto loop_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);
                                                        top_for_stmt->PushLoopQuad(loop_quad);
                                                        stmt_stack.push_back(LOOP_t);
                                                    }
            stmt                                    {
                                                        auto top_for_stmt = for_stmts.top();
                                                        auto expr_jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr); 

                                                        top_for_stmt->PushLoopQuad(expr_jump_quad);
                                                        top_for_stmt->PatchLoopQuads();
                                                        top_for_stmt->PatchBreakJumpQuads(NextQuadLabel());
                                                        top_for_stmt->PatchContinueJumpQuads();

                                                        for_stmts.pop();
                                                        loop_stmts.pop(); 

                                                        stmt_stack.pop_back();

                                                        //ResetTemp();
                                                        DLOG("forstmt -> FOR ( elist ; expr ; elist ) stmt"); 
                                                    }
            ;

returnstmt: RETURN      {
                            if (!InFuncDef()) {
                                SignalError("Invalid return, used outside a function block");
                            } else {
                                auto top_func_def = func_def_stmts.top();
                                Emit(RET_t, nullptr, nullptr, nullptr);
                                auto return_jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);

                                top_func_def->PushReturnJumpQuad(return_jump_quad);
                            }
                        } 
            ';'         {
                            DLOG("returnstmt -> RETURN;"); 
                        }
            | RETURN    
            expr ';'    {
                            if ($2->get_type() == BOOL) {
                                $2 = ConcludeShortCircuit(BOOL_EXPR_CAST($2));
                            }

                            if (!InFuncDef()) 
                                SignalError("Invalid return, used outside a function block");
                            else {
                                auto top_func_def = func_def_stmts.top();
                                Emit(RET_t, $2, nullptr, nullptr);
                                auto return_jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);

                                top_func_def->PushReturnJumpQuad(return_jump_quad);
                                DLOG("returnstmt -> RETURN expr;");
                            }    
                        }
            ;

%%

int yyerror(std::string yaccProvidedMessage) {
    SignalError(yaccProvidedMessage);
    return 1;
}
 
int main(int argc, char** argv) {
    //Can't reach me for I am... above (LUL)
    InitLibraryFunctions();

    if (argc > 1) {
        if (!(yyin = fopen(argv[1], "r"))) {
            fprintf(stderr, "Cannot read file: %s\n", argv[1]);
            return 1;
        }
    }
    else {
        yyin = stdin;
    }
    yyparse();
    #if defined LOGQUADS
        if (NoErrorSignaled()) {
            LogQuads(std::cout);
        }
    #endif 
    #if defined LOGSYMTABLE
        if (NoErrorSignaled()) 
            LogSymTable(std::cout);
    #endif
    #if defined LOGQUADSTXT
        if (NoErrorSignaled()) {
            const char *path="../quads.txt";
            std::ofstream quad_file(path);
            LogQuads(quad_file);
            quad_file.close();
        }
    #endif  

    return 0;
}

bool error_flag = false;

inline bool NoErrorSignaled() {
    return error_flag == false;
}  

void SignalError(std::string msg) {
    #if !defined TEST
        std::cout << "\033[31mError, in line: " << yylineno << ":\033[0m " << msg << std::endl;
    #else
        std::cout << "Error, in line: " << yylineno << ": " << msg << std::endl; 
    #endif    
    error_flag = 1;
}

void LogWarning(std::string msg) {
    #if !defined TEST
        std::cout << "\033[33mWarning, in line: " << yylineno << ":\033[0m " << msg << std::endl;
    #else
        std::cout << "Warning, in line: " << yylineno << ": " << msg << std::endl ;
    #endif
}

inline void LogSymTable(std::ostream& output) {
    output << symbol_table;
}

void LogQuads(std::ostream& output) {
    for (auto quad : quads) {
        output << *quad << std::endl;
    }
}

void IncreaseScope() {
    Block* new_block = new Block();
    symbol_table.Insert(++current_scope, new_block);
    program_stack.Push(new_block);
}

void DecreaseScope() {
    program_stack.Top()->Deactivate();
    program_stack.Pop();
    --current_scope;
}

void HideLowerScopes() {
    if (current_scope != global_scope)
        program_stack.Top()->Deactivate();
    if (current_scope > 1)
        program_stack.DeactivateLowerScopes();
}

void DefineSymbol(Symbol* symbol) {
    assert(symbol != nullptr);
    program_stack.Top()->Insert(symbol);
}

void InitLibraryFunctions() {
    IncreaseScope(); 
    DefineSymbol(new Symbol(LIB_FUNC, "print", LIB_FUNC_LINE, global_scope, PROGRAM_VAR, program_var_offset++, nullptr));
    DefineSymbol(new Symbol(LIB_FUNC, "input", LIB_FUNC_LINE, global_scope, PROGRAM_VAR, program_var_offset++, nullptr));
    DefineSymbol(new Symbol(LIB_FUNC, "objectmemberkeys", LIB_FUNC_LINE, global_scope, PROGRAM_VAR, program_var_offset++, nullptr));
    DefineSymbol(new Symbol(LIB_FUNC, "objecttotalmembers", LIB_FUNC_LINE, global_scope, PROGRAM_VAR, program_var_offset++, nullptr));
    DefineSymbol(new Symbol(LIB_FUNC, "objectcopy", LIB_FUNC_LINE, global_scope, PROGRAM_VAR, program_var_offset++, nullptr));
    DefineSymbol(new Symbol(LIB_FUNC, "totalarguments", LIB_FUNC_LINE, global_scope, PROGRAM_VAR, program_var_offset++, nullptr));
    DefineSymbol(new Symbol(LIB_FUNC, "argument", LIB_FUNC_LINE, global_scope, PROGRAM_VAR, program_var_offset++, nullptr));
    DefineSymbol(new Symbol(LIB_FUNC, "typeof", LIB_FUNC_LINE, global_scope, PROGRAM_VAR, program_var_offset++, nullptr));
    DefineSymbol(new Symbol(LIB_FUNC, "strtonum", LIB_FUNC_LINE, global_scope, PROGRAM_VAR,program_var_offset++, nullptr));
    DefineSymbol(new Symbol(LIB_FUNC, "sqrt", LIB_FUNC_LINE, global_scope, PROGRAM_VAR, program_var_offset++, nullptr));
    DefineSymbol(new Symbol(LIB_FUNC, "cos", LIB_FUNC_LINE, global_scope, PROGRAM_VAR, program_var_offset++, nullptr));
    DefineSymbol(new Symbol(LIB_FUNC, "sin", LIB_FUNC_LINE, global_scope, PROGRAM_VAR, program_var_offset++, nullptr));
}

Symbol* NewSymbol(ExprType type, const char* id, Expression* index) {
    assert (id != nullptr);
    if (InFuncDef()) {
        auto new_symbol = new Symbol(type, id, yylineno, current_scope, FUNCTION_LOCAL, func_def_stmts.top()->get_offset(), index);
        func_def_stmts.top()->IncreaseOffset();
        
        return new_symbol;
    }
    else {
        return new Symbol(type, id, yylineno, current_scope, PROGRAM_VAR, program_var_offset++, index);
    }
}

Symbol* DefineNewSymbol(ExprType type, const char* id, Expression* index) {
    assert(id != nullptr);
    auto new_symbol = NewSymbol(type, id, index);
    DefineSymbol(new_symbol);

    return new_symbol;
}

unsigned int anonymus_funcs_counter = 0;

std::string NewAnonymousFuncName() {
    std::string an = "$";
    an += std::to_string(anonymus_funcs_counter++);

    return an;
}

Symbol* NewAnonymousFunc() {
    return NewSymbol(USER_FUNC, NewAnonymousFuncName().c_str(), nullptr);
}

Symbol* DefineNewAnonymousFunc() {
    auto new_an_func = NewAnonymousFunc();
    DefineSymbol(new_an_func);

    return new_an_func;
}

std::list<Symbol*>  stashed_formal_arguments;

void DefineStashedFormalArguments() { 
    for (auto i : stashed_formal_arguments) {
        DefineSymbol(i);
    }
    stashed_formal_arguments.clear();
}

void StashFormalArgument(Symbol* symbol) {
    assert(symbol != nullptr);
    stashed_formal_arguments.push_back(symbol);
}

unsigned int temp_counter = 0;

inline std::string NewTempName() {
    return  "^" + std::to_string(temp_counter++); 
}

inline void ResetTemp() { 
    temp_counter = 0; 
}

Symbol* NewTemp(ExprType type, Expression* index) {
    std::string id = NewTempName();

    auto new_temp = program_stack.Top()->Lookup(id);
    
    if (new_temp == nullptr)  
        new_temp = DefineNewSymbol(type, id.c_str(), index);

    return new_temp;
}

Quad* Emit(Iopcode op, Expression* result, Expression* arg1, Expression* arg2) {
    unsigned int label = quads.size() + 1;
    Quad* q = new quad(op, result, arg1, arg2, label, yylineno);
    quads.push_back(q);

    return q;
}

Symbol* EmitIfTableItem(Symbol* sym) {
    assert (sym != nullptr);
    if (!IsTableItem(sym)) {
        return sym;
    }
    else {
        auto temp = NewTemp(TABLE_ITEM, sym->get_index());
        Emit(TABLEGETELEM_t, temp, sym, sym->get_index());

        return temp;
    }    
}

Symbol* MemberItem(Symbol* sym, const char* id) {
    assert(sym != nullptr);
    assert(id != nullptr);
    sym = EmitIfTableItem(sym);
    auto index = new StringConstant(std::string(id));

    return DefineNewSymbol(TABLE_ITEM, sym->get_id().c_str(), index);
}

void checkValidCall(Symbol* called_symbol, std::list<Expression*> params) {
    assert (called_symbol != nullptr);
    if (IsUserFunction(called_symbol)) {
        auto call_args_num = params.size();
        auto func_def_args_num = called_symbol->get_formal_arguments().size();
        if (call_args_num < func_def_args_num) 
            SignalError("Too few arguments passed to function: " + called_symbol->get_id() + ", defined in line: " + std::to_string(called_symbol->get_line()));
        else if (call_args_num > func_def_args_num) 
            LogWarning("Too many arguments passed to function: " + called_symbol->get_id() + ", defined in line: " + std::to_string(called_symbol->get_line()));
    }
}

Call* MakeCall(Symbol* called_symbol, CallSuffix* call_suffix) {
    assert (called_symbol != nullptr);
    assert (call_suffix != nullptr);
    auto params = call_suffix->get_elist()->exprs;

    checkValidCall(called_symbol, params);

    called_symbol = EmitIfTableItem(called_symbol);
    for (auto param : params)
        Emit(PARAM_t, param, nullptr, nullptr);

    auto return_value = NewTemp(VAR, nullptr);
    Emit(CALL_t, called_symbol, nullptr, nullptr);
    Emit(GETRETVAL_t, return_value, nullptr, nullptr);

    return new Call(called_symbol, call_suffix, return_value); 
}

unsigned int NextQuadLabel() {
    if (quads.size() == 0)
        return 1;
    else     
        return quads.back()->label + 1;
}

inline bool IsLibraryFunction(Expression* expr) {
    return expr->get_type() == LIB_FUNC; 
}

inline bool IsUserFunction(Expression* expr) {
    return expr->get_type() == USER_FUNC; 
}

inline bool IsVariable(Expression* expr) {
    return expr->get_type() == VAR;
}

inline bool IsConstString(Expression* expr) {
    return expr->get_type() == CONST_STR;
}

inline bool IsConstBool(Expression* expr) {
    return expr->get_type() == CONST_BOOL;
}

inline bool IsTableMake(Expression* expr) {
    return expr->get_type() == TABLE_MAKE;
}

inline bool IsGlobalVar(Symbol* symbol) { 
    return IsVariable(symbol) && symbol->get_scope() == global_scope; 
}

inline bool IsAtCurrentScope(Symbol* symbol) {
    return symbol->get_scope() == current_scope;
}

inline bool IsTableItem(Symbol* symbol) {
    return symbol->get_type() == TABLE_ITEM;
}

inline bool IsMethodCall(CallSuffix* call_suffix) {
    return call_suffix->get_type() == METHOD_CALL;
}
    
inline bool InLoop() {
    return loop_stmts.size() != 0; 
}

inline bool InFuncDef() { 
    return func_def_stmts.size() != 0; 
} 

bool IsValidArithmetic(Expression* expr, std::string context) {
    assert (expr != nullptr);
    if (IsLibraryFunction(expr)) {
        SignalError("Invalid use of " + context + " operator on library function " + expr->to_string());
        return false;
    }
    else if (IsUserFunction(expr)) {
        SignalError("Invalid use of " + context + " operator on user function " + expr->to_string());
        return false;
    }
    else if (IsConstString(expr)) {
        SignalError("Invalid use of " + context + " operator on const string " + expr->to_string());
        return false;
    }
    else if (IsConstBool(expr)) {
        SignalError("Invalid use of " + context + " operator on const bool " + expr->to_string());
        return false;
    }
    else if (IsTableMake(expr)) {
        SignalError("Invalid use of " + context + " operator on table " + expr->to_string());
        return false;
    }

    return true;                
}

inline bool IsValidArithmeticOp(Expression* expr) {
    assert(expr != nullptr);
    return IsValidArithmetic(expr, std::string("arithmetic"));
}

inline bool IsValidArithmeticComp(Expression* expr) {
    assert(expr != nullptr);
    return IsValidArithmetic(expr, std::string("comparison"));
}

bool IsValidAssign(Symbol* left_operand) {
    assert(left_operand != nullptr);
    if (IsUserFunction(left_operand) || IsLibraryFunction(left_operand)) {
        SignalError("Functions are constant their value cannot be changed");

        return false;
    }

    return true;                
}

void
BackPatch(std::list<unsigned int> l_list, unsigned int q_label) {
    for (unsigned int i : l_list) {
        if (quads[i-1]->op == JUMP_t) {
            PatchJumpQuad(quads[i-1], q_label);
        }
        else{
            PatchBranchQuad(quads[i-1], q_label);
        }
    }
}

Symbol*
ConcludeShortCircuit(BoolExpr* expr) {
    auto temp = NewTemp(VAR, nullptr);

    BackPatch(expr->true_list, NextQuadLabel());
    Emit(ASSIGN_t, temp, new BoolConstant(true), nullptr);

    Quad* jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);
    PatchJumpQuad(jump_quad, jump_quad->label + 2);

    BackPatch(expr->false_list, NextQuadLabel());
    Emit(ASSIGN_t, temp, new BoolConstant(false), nullptr);

    return temp;
}

bool
IsValidBreakContinue() {
    return (stmt_stack.empty() || stmt_stack.back() != LOOP_t) ? false : true;
}
