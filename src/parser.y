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
    #include "../include/expression/call.h"
    #include "../include/expression/tablemake.h"
    #include "../include/expression/tablemake_elems.h"
    #include "../include/expression/tablemake_pairs.h"
    #include "../include/symbol_table.h"
    #include "../include/program_stack.h"
    #include "../include/instruction_opcodes.h"
    #include "../include/quad.h"
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

    #define OUT_OF_SCOPE   -1
    #define LIB_FUNC_LINE   0
    #define TEMP_LINE       0

    const unsigned int          global_scope = 0;
    unsigned int                current_scope = OUT_OF_SCOPE;
    
    SymbolTable                 symbol_table;
    ProgramStack                program_stack;  

    unsigned int                program_var_offset = 0;

    std::vector<Quad*>          quads;
    
    std::stack<LoopStmt*>       loop_stmts;
    std::stack<WhileStmt*>      while_stmts;
    std::stack<ForStmt*>        for_stmts;

    std::stack<FuncDefStmt*>    func_def_stmts;  

    std::stack<IfStmt*>         if_stmts;

    std::stack<Call*>           call_exprs;
    std::stack<TableMakeElems*> tablemake_elems_exprs;
    std::stack<TableMakePairs*> tablemake_pairs_exprs;

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

    bool                        InLoop();
    bool                        InFuncDef();
    bool                        InCall();
    bool                        InTableMakeElems();

    bool                        IsValidArithmetic(Expression* expr);
    bool                        IsValidAssign(Symbol* left_operand);
    bool                        IsValidBoolExpr(Expression* expr1, Expression* expr2);
%}

%union {                                                    
    char*                       stringValue;
    int                         intValue;
    double                      doubleValue;

    class FuncDefStmt*          funcdef;

    class Expression*           expr;

    class AssignExpr*           assignexpr;

    class Primary*              prim;
    class Constant*             con;
    class Call*                 call;
    class Symbol*               sym;
    class TableMake*            tablemake;
}

%start program

%token EQUAL NOTEQUAL COLONCOLON DOTDOT GEQL LEQL MINUSMINUS PLUSPLUS
%token LOCAL FUNCTION IF ELSE FOR WHILE BREAK CONTINUE NIL TRUE FALSE RETURN

%token <stringValue>    STRING ID 
%token <intValue>       INTNUM
%token <doubleValue>    DOUBLENUM

%type <funcdef>     funcdef 

%type <expr>        term expr

%type <assignexpr>  assignexpr

%type <prim>        primary
%type <con>         const
%type <sym>         lvalue member
%type <call>        call
%type <tablemake>   objectdef

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
                                        ResetTemp();
                                    }
            stmts                   {
                                        DLOG("stmts -> stmt stmts");
                                    }
            |                       {
                                        DLOG("stmts -> EMPTY");
                                    }
            ;

stmt:         expr ';'              {
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
                                        if(!InLoop()) {
                                            SignalError("invalid keyword BREAK outside of loop");
                                        } else {
                                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);

                                            auto top_loop_stmt = loop_stmts.top();
                                            top_loop_stmt->PushBreakJumpQuad(jump_quad);
                                        }

                                        DLOG("stmt -> break;");
                                    }
            | CONTINUE ';'          {
                                        if(!InLoop()) {
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

                                        if (IsValidArithmetic(expr1) & IsValidArithmetic(expr2)) {
                                            auto temp = NewTemp(VAR, nullptr);
                                            Emit(ADD_t, temp, expr1, expr2);

                                            $$ = new ArithmeticExpr(temp, expr1, expr2);
                                        }
                                        DLOG("expr -> expr + expr");
                                    }
            | expr '-' expr         {
                                        auto expr1 = $1;
                                        auto expr2 = $3;

                                        if (IsValidArithmetic(expr1) & IsValidArithmetic(expr2)) {
                                            auto temp = NewTemp(VAR, nullptr);
                                            Emit(SUB_t, temp, expr1, expr2);

                                            $$ = new ArithmeticExpr(temp, expr1, expr2);
                                        } 
                                        DLOG("expr -> expr - expr");
                                    }
            | expr '*' expr         {
                                        auto expr1 = $1;
                                        auto expr2 = $3;

                                        if (IsValidArithmetic(expr1) & IsValidArithmetic(expr2)) {
                                            auto temp = NewTemp(VAR, nullptr);
                                            Emit(MUL_t, temp, expr1, expr2);

                                            $$ = new ArithmeticExpr(temp, expr1, expr2);
                                        }
                                        DLOG("expr -> expr * expr");
                                    }
            | expr '/' expr         {
                                        auto expr1 = $1;
                                        auto expr2 = $3;

                                        if (IsValidArithmetic(expr1) & IsValidArithmetic(expr2)) {
                                            auto temp = NewTemp(VAR, nullptr);
                                            Emit(DIV_t, temp, expr1, expr2);

                                            $$ = new ArithmeticExpr(temp, expr1, expr2);
                                        }
                                        DLOG("expr -> expr / expr");
                                    }
            | expr '%' expr         {
                                        auto expr1 = $1;
                                        auto expr2 = $3;

                                        if (IsValidArithmetic(expr1) & IsValidArithmetic(expr2)) {
                                            auto temp = NewTemp(VAR, nullptr);
                                            Emit(MOD_t, temp, expr1, expr2);

                                            $$ = new ArithmeticExpr(temp, expr1, expr2);
                                        } 
                                        DLOG("expr -> expr % expr");
                                    }
            | expr '>' expr         {
                                        auto expr1 = $1;
                                        auto expr2 = $3;

                                        if (IsValidBoolExpr(expr1, expr2)) {
                                            auto greater_quad = Emit(IF_GREATER_t, expr1, expr2, nullptr);
                                            PatchBranchQuad(greater_quad, greater_quad->label + 2);

                                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);
                                            PatchJumpQuad(jump_quad, jump_quad->label + 3);

                                            auto temp = NewTemp(VAR, nullptr);

                                            Emit(ASSIGN_t, temp, new BoolConstant(true), nullptr);

                                            jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);
                                            PatchJumpQuad(jump_quad, jump_quad->label + 2);

                                            Emit(ASSIGN_t, temp, new BoolConstant(false), nullptr);

                                            $$ = new BoolExpr(temp, expr1, expr2);
                                        }
                                        DLOG("expr -> expr > expr");
                                    }
            | expr GEQL expr        {
                                        auto expr1 = $1;
                                        auto expr2 = $3;

                                        if (IsValidBoolExpr(expr1, expr2)) {
                                            auto greater_equal_quad = Emit(IF_GREATEREQ_t, expr1, expr2, nullptr);
                                            PatchBranchQuad(greater_equal_quad, greater_equal_quad->label + 2);

                                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);
                                            PatchJumpQuad(jump_quad, jump_quad->label + 3);

                                            auto temp = NewTemp(VAR, nullptr);

                                            Emit(ASSIGN_t, temp, new BoolConstant(true), nullptr);

                                            jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);
                                            PatchJumpQuad(jump_quad, jump_quad->label + 2);

                                            Emit(ASSIGN_t, temp, new BoolConstant(false), nullptr);

                                            $$ = new BoolExpr(temp, expr1, expr2);
                                        }
                                        DLOG("expr -> expr >= expr");
                                    }
            | expr '<' expr         {
                                        auto expr1 = $1;
                                        auto expr2 = $3;

                                        if (IsValidBoolExpr(expr1, expr2)) {
                                            auto less_quad = Emit(IF_LESS_t, expr1, expr2, nullptr);
                                            PatchBranchQuad(less_quad, less_quad->label + 2);

                                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);
                                            PatchJumpQuad(jump_quad, jump_quad->label + 3);

                                            auto temp = NewTemp(VAR, nullptr);

                                            Emit(ASSIGN_t, temp, new BoolConstant(true), nullptr);

                                            jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);
                                            PatchJumpQuad(jump_quad, jump_quad->label + 2);

                                            Emit(ASSIGN_t, temp, new BoolConstant(false), nullptr);

                                            $$ = new BoolExpr(temp, expr1, expr2);
                                        }
                                        DLOG("expr -> expr + expr");
                                    }
            | expr LEQL expr        {
                                        auto expr1 = $1;
                                        auto expr2 = $3;

                                        if (IsValidBoolExpr(expr1, expr2)) {
                                            auto less_equal_quad = Emit(IF_LESSEQ_t, expr1, expr2, nullptr);
                                            PatchBranchQuad(less_equal_quad, less_equal_quad->label + 2);

                                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);
                                            PatchJumpQuad(jump_quad, jump_quad->label + 3);

                                            auto temp = NewTemp(VAR, nullptr);

                                            Emit(ASSIGN_t, temp, new BoolConstant(true), nullptr);

                                            jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);
                                            PatchJumpQuad(jump_quad, jump_quad->label + 2);

                                            Emit(ASSIGN_t, temp, new BoolConstant(false), nullptr);

                                            $$ = new BoolExpr(temp, expr1, expr2);
                                        }
                                        DLOG("expr -> expr <= expr");
                                    }
            | expr EQUAL expr       {
                                        auto expr1 = $1;
                                        auto expr2 = $3;

                                        if (IsValidBoolExpr(expr1, expr2)) {
                                            auto equal_quad = Emit(IF_EQ_t, $1, $3, nullptr);
                                            PatchBranchQuad(equal_quad, equal_quad->label + 2);

                                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);
                                            PatchJumpQuad(jump_quad, jump_quad->label + 3);

                                            auto temp = NewTemp(VAR, nullptr);

                                            Emit(ASSIGN_t, temp, new BoolConstant(true), nullptr);

                                            jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);
                                            PatchJumpQuad(jump_quad, jump_quad->label + 2);

                                            Emit(ASSIGN_t, temp, new BoolConstant(false), nullptr);

                                            $$ = new BoolExpr(temp, expr1, expr2);
                                        }

                                        DLOG("expr -> expr == expr");
                                    }
            | expr NOTEQUAL expr    {
                                        auto expr1 = $1;
                                        auto expr2 = $3;

                                        if (IsValidBoolExpr(expr1, expr2)) {
                                            auto not_equal_quad = Emit(IF_NOTEQ_t,  expr1, expr2, nullptr);
                                            PatchBranchQuad(not_equal_quad, not_equal_quad->label + 2);

                                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);
                                            PatchJumpQuad(jump_quad, jump_quad->label + 3);

                                            auto temp = NewTemp(VAR, nullptr);

                                            Emit(ASSIGN_t, temp, new BoolConstant(true), nullptr);

                                            jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);
                                            PatchJumpQuad(jump_quad, jump_quad->label + 2);

                                            Emit(ASSIGN_t, temp, new BoolConstant(false), nullptr);

                                            $$ = new BoolExpr(temp, expr1, expr2);
                                        } 
                                        DLOG("expr -> expr != expr");
                                    }
            | expr AND expr         {
                                        auto expr1 = $1;
                                        auto expr2 = $3;

                                        if (IsValidBoolExpr(expr1, expr2)) {  //TODO

                                        }
                                        DLOG("expr -> expr and expr");
                                    }
            | expr OR expr          {
                                        auto expr1 = $1;
                                        auto expr2 = $3;
                                        
                                        if (IsValidBoolExpr(expr1, expr2)) {  //TODO
                                        }
                                        DLOG("expr -> assignexpr");
                                    }
            | term                  {
                                        $$ = $1;
                                        DLOG("expr -> term");
                                    }
            ;

term:         '(' expr ')'          {
                                        $$ = $2;
                                        DLOG("term -> (expr)");
                                    }
            | '-' expr %prec UMINUS {
                                        auto symbol = $2;
                                        if (symbol->get_type() == CONST_BOOL) {
                                            SignalError("Illegal use of unary minus on constant boolean");
                                        }
                                        else {
                                            auto temp = NewTemp(VAR, nullptr);
                                            Emit(UMINUS_t, temp, symbol, nullptr);
                                            $$ = symbol;
                                        }

                                        DLOG("term -> -expr");
                                    }
            | NOT expr              {
                                        auto equal_quad = Emit(IF_EQ_t, $2, new BoolConstant(true),  nullptr);
                                        PatchBranchQuad(equal_quad, equal_quad->label + 4);

                                        auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);
                                        PatchJumpQuad(jump_quad, jump_quad->label + 1);

                                        auto temp = NewTemp(VAR, nullptr);

                                        Emit(ASSIGN_t, temp, new BoolConstant(true), nullptr);

                                        jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);
                                        PatchJumpQuad(jump_quad, jump_quad->label + 2);

                                        Emit(ASSIGN_t, temp, new BoolConstant(false), nullptr);

                                        $$ = temp;
                                        DLOG("term -> not expr");
                                    }
            | PLUSPLUS lvalue       {
                                        auto symbol = $2;
                                        Symbol* result;
                                        if (IsValidArithmetic(symbol)) {
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
                                        if (IsValidArithmetic(symbol)) {
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
                                        if (IsValidArithmetic(symbol)) {
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
                                        if (IsValidArithmetic(symbol)) {
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
                                                DLOG("call -> call(elist)");
                                            }
            | lvalue                        {
                                                
                                                auto called_symbol = $1;
                                                auto call = new Call(called_symbol);
                                                
                                                call_exprs.push(call);

                                                $<call>$ = call;
                                            }
            callsuffix                      {
                                                auto top_call = call_exprs.top();
                                                auto called_symbol = top_call->get_called_symbol();

                                                auto temp_value = NewTemp(VAR, nullptr);
                        
                                                Emit(CALL_t, called_symbol, nullptr, nullptr);    
                                                Emit(GETRETVAL_t, temp_value, nullptr, nullptr);

                                                top_call->set_ret_val(temp_value);

                                                if (IsLibraryFunction(called_symbol) || IsUserFunction(called_symbol)) {
                                                    auto args_num = called_symbol->get_formal_arguments().size();
                                                    auto call_args_num = top_call->get_params().size();

                                                    if (call_args_num < args_num)
                                                        SignalError("Too few arguments passed to function: " + called_symbol->get_id() + ", defined in line: " + std::to_string(called_symbol->get_line()));
                                                    else if (call_args_num > args_num)
                                                        LogWarning("Too many arguments passed to function: " + called_symbol->get_id() + ", defined in line: " + std::to_string(called_symbol->get_line()));
                                                }

                                                call_exprs.pop();

                                                $<call>$ = top_call;

                                                DLOG("call -> lvalue callsuffix");
                                            }
            | '(' funcdef ')'               {                       
                                                auto func_def = $2;                         
                                                auto called_symbol = func_def->get_sym();
                                                auto call = new Call(called_symbol);

                                                call_exprs.push(call);

                                                $<call>$ = call;
                                            }
            '(' elist ')'                   {
                                                auto top_call = call_exprs.top();
                                                auto called_symbol = top_call->get_called_symbol();

                                                auto temp_value = NewTemp(VAR, nullptr);

                                                Emit(CALL_t, called_symbol, nullptr, nullptr);
                                                Emit(GETRETVAL_t, temp_value, nullptr, nullptr);

                                                top_call->set_ret_val(temp_value);

                                                if (IsLibraryFunction(called_symbol) || IsUserFunction(called_symbol)) {
                                                    auto args_num = called_symbol->get_formal_arguments().size();
                                                    auto call_args_num = top_call->get_params().size();

                                                    if (call_args_num < args_num)
                                                        SignalError("Too few arguments passed to function: " + called_symbol->get_id() + ", defined in line: " + std::to_string(called_symbol->get_line()));
                                                    else if (call_args_num > args_num)
                                                        LogWarning("Too many arguments passed to function: " + called_symbol->get_id() + ", defined in line: " + std::to_string(called_symbol->get_line()));
                                                }

                                                call_exprs.pop();

                                                $<call>$ = top_call;

                                                DLOG("call -> (funcdef)(elist)");
                                            }
            ;

callsuffix: normcall        {
                                DLOG("callsuffix -> normcall");
                            }
            | methodcall    {
                                DLOG("callsuffix -> methodcall");
                            }
            ;

normcall:   '(' elist ')'   {
                                DLOG("normcall -> (elist)"); 
                            }
            ;

methodcall: DOTDOT ID '(' elist ')' {
                                        DLOG("methodcall -> ..id(elist)");
                                    }
            ;

multelist:  ',' expr multelist  {
                                    if (InCall()) {
                                        auto top_call = call_exprs.top();
                                        top_call->IncludeParameter($2);
                                        Emit(PARAM_t, $2, nullptr, nullptr);
                                    } 
                                    if (InTableMakeElems()) {
                                        auto top_tablemake_elems_expr = tablemake_elems_exprs.top();
                                        top_tablemake_elems_expr->AddElement($2);
                                    }

                                    DLOG("multelist -> ,expr multelist");
                                }
            |                   {
                                    DLOG("multelist -> EMPTY");
                                }
            ;

elist:      expr multelist  {
                                if (InCall()) {
                                    auto top_call = call_exprs.top();
                                    top_call->IncludeParameter($1);
                                    Emit(PARAM_t, $1, nullptr, nullptr);
                                }
                                if (InTableMakeElems()) {
                                    auto top_tablemake_elems_expr = tablemake_elems_exprs.top();
                                    top_tablemake_elems_expr->AddElement($1);
                                }
                                             
                                DLOG("elist -> expr multelist");
                            }
            |               {
                                DLOG("elist -> EMPTY");
                            }
            ;

objectdef:  '['                 {
                                    auto tablemake_elems_expr = new TableMakeElems();
                                    tablemake_elems_exprs.push(tablemake_elems_expr);
                                }
             elist ']'          {
                                    auto temp = NewTemp(VAR, nullptr);
                                    Emit(TABLECREATE_t, temp, nullptr, nullptr);

                                    auto top_tablemake_elems_expr = tablemake_elems_exprs.top();

                                    top_tablemake_elems_expr->set_table(temp);

                                    unsigned int elem_cnt = 0;
                                    for (auto element : top_tablemake_elems_expr->get_elements())
                                        Emit(TABLESETELEM_t, temp, new IntConstant(elem_cnt++), element);

                                    tablemake_elems_exprs.pop();  

                                    $$ = top_tablemake_elems_expr; 

                                    DLOG("objectdef -> [elist]");
                                }
            | '['               {
                                    auto tablemake_pairs_expr = new TableMakePairs();
                                    tablemake_pairs_exprs.push(tablemake_pairs_expr);
                                }
            indexed ']'         { 
                                    auto temp = NewTemp(VAR, nullptr);
                                    Emit(TABLECREATE_t, temp, nullptr, nullptr);

                                    auto top_tablemake_pairs_expr = tablemake_pairs_exprs.top();

                                    top_tablemake_pairs_expr->set_table(temp);

                                    for (auto pair : top_tablemake_pairs_expr->get_pairs())
                                        Emit(TABLESETELEM_t, temp, pair.first, pair.second);

                                    tablemake_pairs_exprs.pop();

                                    $$ = top_tablemake_pairs_expr;
                                        
                                    DLOG("objectdef -> [indexed]");
                                }
            ;

multindexed:',' indexedelem multindexed {
                                            DLOG("multindexed -> , indexedelem multidexed"); 
                                        }
            |                           {
                                            DLOG("elsestmt -> EMPTY");
                                        }
            ;

indexed:    indexedelem multindexed {
                                        DLOG("indexed -> indexedelem multidexed"); 
                                    }
            ;

indexedelem:'{' expr ':' expr '}'   {
                                        auto top_tablemake_pairs_expr = tablemake_pairs_exprs.top();
                                        top_tablemake_pairs_expr->AddPair($2, $4);

                                        DLOG("indexedelem -> { expr : expr }"); 
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
                            }
            block           {
                                auto top_func_def = func_def_stmts.top();
                                auto anonymous_function = top_func_def->get_sym();
                                
                                auto func_end_quad = Emit(FUNCEND_t, anonymous_function, nullptr, nullptr);

                                top_func_def->PatchFuncStartJumpQuad(func_end_quad->label + 1);
                                top_func_def->PatchReturnJumpQuads(func_end_quad->label);

                                program_stack.ActivateLowerScopes();

                                func_def_stmts.pop();

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
                            }
            block           { 
                                auto top_func_def =  func_def_stmts.top();
                                auto function = top_func_def->get_sym();
                                
                                auto func_end_quad = Emit(FUNCEND_t, function, nullptr, nullptr);
                                
                                top_func_def->PatchFuncStartJumpQuad(func_end_quad->label + 1);
                                top_func_def->PatchReturnJumpQuads(func_end_quad->label);

                                func_def_stmts.pop();
                                
                                program_stack.ActivateLowerScopes();

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
                                                auto if_stmt = new IfStmt();
                                                if_stmts.push(if_stmt);

                                                auto branch_quad = Emit(IF_EQ_t, $3, new BoolConstant(true), nullptr);
                                                auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr); 

                                                PatchBranchQuad(branch_quad, branch_quad->label + 2);
                                                if_stmt->set_if_jump_quad(jump_quad);
                                            }
            stmt                            {
                                                ResetTemp();
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

                                ResetTemp();

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
                                    auto top_while_stmt = while_stmts.top();

                                    auto branch_quad = Emit(IF_EQ_t, $4, new BoolConstant(true), nullptr);
                                    auto exit_quad = Emit(JUMP_t, nullptr, nullptr, nullptr);

                                    top_while_stmt->PushLoopQuad(branch_quad);
                                    top_while_stmt->PushLoopQuad(exit_quad);
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

                                    ResetTemp();

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

                                                        ResetTemp();

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
    if (argc > 1) {
        if (!(yyin = fopen(argv[1], "r"))) {
            fprintf(stderr, "Cannot read file: %s\n", argv[1]);
            return 1;
        }
    }
    else {
        yyin = stdin;
    }
    
    InitLibraryFunctions();

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
    an += std::to_string(++anonymus_funcs_counter);

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
    
inline bool InLoop() {
    return loop_stmts.size() != 0; 
}

inline bool InFuncDef() { 
    return func_def_stmts.size() != 0; 
} 

inline bool InCall() {
    return call_exprs.size() != 0;
}

inline bool InTableMakeElems() { 
    return tablemake_elems_exprs.size() != 0; 
}

bool IsValidArithmetic(Expression* expr) {
    assert (expr != nullptr);
    if (IsLibraryFunction(expr)) {
        SignalError("Invalid use of arithmetic operator on library function " + expr->to_string());
        return false;
    }
    else if (IsUserFunction(expr)) {
        SignalError("Invalid use of arithmetic operator on user function " + expr->to_string());
        return false;
    }
    else if (IsConstString(expr)) {
        SignalError("Invalid use of arithmetic operator on const string " + expr->to_string());
        return false;
    }
    else if (IsConstBool(expr)) {
        SignalError("Invalid use of arithmetic operator on const bool " + expr->to_string());
        return false;
    }
    else if (IsTableMake(expr)) {
        SignalError("Invalid use of arithmetic operator on table " + expr->to_string());
        return false;
    }

    return true;                
}

bool IsValidAssign(Symbol* left_operand) {
    assert(left_operand != nullptr);
    if (IsUserFunction(left_operand) || IsLibraryFunction(left_operand)) {
        SignalError("Functions are constant their value cannot be changed");

        return false;
    }

    return true;
}

bool IsValidBoolExpr(Expression* expr1, Expression* expr2) {
    bool is_valid = true;

    if (IsLibraryFunction(expr1)) {
        SignalError("Invalid use of comparison operator on library function " + expr1->to_string());
        is_valid = false;
    }
    if (IsLibraryFunction(expr2)) {
        SignalError("Invalid use of comparison operator on library function " + expr2->to_string());
        is_valid = false;
    }
    if (IsUserFunction(expr1)) {
        SignalError("Invalid use of comparison operator on user function " + expr1->to_string());
        is_valid = false;
    }
    if (IsUserFunction(expr2)) {
        SignalError("Invalid use of comparison operator on user function " + expr2->to_string());
        is_valid = false;
    }
    if (IsConstString(expr1)) {
        SignalError("Invalid use of comparison operator on const string " + expr1->to_string());
        is_valid = false;
    }
    if (IsConstString(expr2)) {
        SignalError("Invalid use of comparison operator on const string " + expr2->to_string());
        is_valid = false;
    }
    if (IsConstBool(expr1)) {
        SignalError("Invalid use of comparison operator on const bool " + expr1->to_string());
        is_valid = false;
    }
    if (IsConstBool(expr2)) {
        SignalError("Invalid use of comparison operator on const bool " + expr2->to_string());
        is_valid = false;
    }

    return is_valid;                
}