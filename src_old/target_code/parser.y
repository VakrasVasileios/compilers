%{
    #include <stdio.h>
    #include <iostream>
    #include <fstream>
    #include <string>
    #include "../../include/target_code/target_code.h"

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

    using namespace expression;
    using namespace syntax_analysis;
    using namespace intermediate_code;
    using namespace target_code;

    #define BOOL_EXPR_CAST(e)   static_cast<BoolExpr*>(e)    
%}

%union {                                                    
    char*                                   stringValue;
    int                                     intValue;
    double                                  doubleValue;

    unsigned int                            quad_label;
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

%type <quad_label>      M funcbody

%type <funcdef>         funcdef

%type <expr>            term expr

%type <assignexpr>      assignexpr

%type <prim>            primary
%type <con>             const
%type <sym>             lvalue member funcid funcprefix
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
                                            ConcludeShortCircuit(BOOL_EXPR_CAST($1), yylineno);
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
                                            SignalError("invalid keyword BREAK outside of loop", yylineno);
                                        } else {
                                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);

                                            auto top_loop_stmt = loop_stmts.top();
                                            top_loop_stmt->PushBreakJumpQuad(jump_quad);
                                        }

                                        DLOG("stmt -> break;");
                                    }
            | CONTINUE ';'          {
                                        if (!IsValidBreakContinue()) {
                                            SignalError("invalid keyword CONTINUE outside of loop", yylineno);
                                        } else {
                                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);

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
                                        if (IsValidArithmeticOp(expr1, yylineno) & IsValidArithmeticOp(expr2, yylineno)) {
                                            auto temp = NewTemp(VAR, nullptr);
                                            Emit(ADD_t, temp, expr1, expr2, yylineno);
                                            $$ = new ArithmeticExpr(temp, expr1, expr2);
                                        }
                                        DLOG("expr -> expr + expr");
                                    }
            | expr '-' expr         {
                                        auto expr1 = $1;
                                        auto expr2 = $3;
                                        if (IsValidArithmeticOp(expr1, yylineno) & IsValidArithmeticOp(expr2, yylineno)) {
                                            auto temp = NewTemp(VAR, nullptr);
                                            Emit(SUB_t, temp, expr1, expr2, yylineno);
                                            $$ = new ArithmeticExpr(temp, expr1, expr2);
                                        } 
                                        DLOG("expr -> expr - expr");
                                    }
            | expr '*' expr         {
                                        auto expr1 = $1;
                                        auto expr2 = $3;
                                        if (IsValidArithmeticOp(expr1, yylineno) & IsValidArithmeticOp(expr2, yylineno)) {
                                            auto temp = NewTemp(VAR, nullptr);
                                            Emit(MUL_t, temp, expr1, expr2, yylineno);
                                            $$ = new ArithmeticExpr(temp, expr1, expr2);
                                        }
                                        DLOG("expr -> expr * expr");
                                    }
            | expr '/' expr         {
                                        auto expr1 = $1;
                                        auto expr2 = $3;
                                        if (IsValidArithmeticOp(expr1, yylineno) & IsValidArithmeticOp(expr2, yylineno)) {
                                            auto temp = NewTemp(VAR, nullptr);
                                            Emit(DIV_t, temp, expr1, expr2, yylineno);
                                            $$ = new ArithmeticExpr(temp, expr1, expr2);
                                        }
                                        DLOG("expr -> expr / expr");
                                    }
            | expr '%' expr         {
                                        auto expr1 = $1;
                                        auto expr2 = $3;
                                        if (IsValidArithmeticOp(expr1, yylineno) & IsValidArithmeticOp(expr2, yylineno)) {
                                            auto temp = NewTemp(VAR, nullptr);
                                            Emit(MOD_t, temp, expr1, expr2, yylineno);
                                            $$ = new ArithmeticExpr(temp, expr1, expr2);
                                        } 
                                        DLOG("expr -> expr % expr");
                                    }
            | expr '>' expr         {
                                        auto expr1 = $1;
                                        auto expr2 = $3;
                                        if (IsValidArithmeticComp(expr1, yylineno) & IsValidArithmeticComp(expr2, yylineno)) {
                                            BoolExpr* n_expr = new BoolExpr(expr1, expr2, nullptr);

                                            auto greater_quad = Emit(IF_GREATER_t, expr1, expr2, nullptr, yylineno);

                                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);

                                            n_expr->true_list.push_back(greater_quad->label);
                                            n_expr->false_list.push_back(jump_quad->label);

                                            $$ = n_expr;
                                        }
                                        DLOG("expr -> expr > expr");
                                    }
            | expr GEQL expr        {
                                        auto expr1 = $1;
                                        auto expr2 = $3;
                                        if (IsValidArithmeticComp(expr1, yylineno) & IsValidArithmeticComp(expr2, yylineno)) {
                                            BoolExpr* n_expr = new BoolExpr(expr1, expr2, nullptr);

                                            auto greater_equal_quad = Emit(IF_GREATEREQ_t, expr1, expr2, nullptr, yylineno);

                                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);

                                            n_expr->true_list.push_back(greater_equal_quad->label);
                                            n_expr->false_list.push_back(jump_quad->label);

                                            $$ = n_expr;
                                        }

                                        DLOG("expr -> expr >= expr");
                                    }
            | expr '<' expr         {
                                        auto expr1 = $1;
                                        auto expr2 = $3;
                                        if (IsValidArithmeticComp(expr1, yylineno) & IsValidArithmeticComp(expr2, yylineno)) {
                                            BoolExpr* n_expr = new BoolExpr(expr1, expr2, nullptr);

                                            auto less_quad = Emit(IF_LESS_t, expr1, expr2, nullptr, yylineno);

                                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);

                                            n_expr->true_list.push_back(less_quad->label);
                                            n_expr->false_list.push_back(jump_quad->label);

                                            $$ = n_expr;
                                        }
                                        DLOG("expr -> expr + expr");
                                    }
            | expr LEQL expr        {
                                        auto expr1 = $1;
                                        auto expr2 = $3;
                                        if (IsValidArithmeticComp(expr1, yylineno) & IsValidArithmeticComp(expr2, yylineno)) {
                                            BoolExpr* n_expr = new BoolExpr(expr1, expr2, nullptr);

                                            auto less_equal_quad = Emit(IF_LESSEQ_t, expr1, expr2, nullptr, yylineno);

                                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);

                                            n_expr->true_list.push_back(less_equal_quad->label);
                                            n_expr->false_list.push_back(jump_quad->label);

                                            $$ = n_expr;
                                        }
                                        DLOG("expr -> expr <= expr");
                                    }
            | expr EQUAL            {
                                        if ($1->get_type() == BOOL) {
                                            $1 = ConcludeShortCircuit(BOOL_EXPR_CAST($1), yylineno);
                                        }
                                    }
            expr                    {
                                        if ($4->get_type() == BOOL) {
                                            $4 = ConcludeShortCircuit(BOOL_EXPR_CAST($4), yylineno);
                                        }

                                        auto expr1 = $1;
                                        auto expr2 = $4;

                                        BoolExpr* n_expr = new BoolExpr(expr1, expr2, nullptr);

                                        auto equal_quad = Emit(IF_EQ_t, expr1, expr2, nullptr, yylineno);

                                        auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);

                                        n_expr->true_list.push_back(equal_quad->label);
                                        n_expr->false_list.push_back(jump_quad->label);

                                        $$ = n_expr;

                                        DLOG("expr -> expr == expr");
                                    }
            | expr NOTEQUAL         {
                                        if ($1->get_type() == BOOL) {
                                            $1 = ConcludeShortCircuit(BOOL_EXPR_CAST($1), yylineno);
                                        }
                                    }
            expr                    {
                                        if ($4->get_type() == BOOL) {
                                            $4 = ConcludeShortCircuit(BOOL_EXPR_CAST($4), yylineno);
                                        }

                                        auto expr1 = $1;
                                        auto expr2 = $4;

                                        BoolExpr* n_expr = new BoolExpr(expr1, expr2, nullptr);

                                        auto not_equal_quad = Emit(IF_NOTEQ_t,  expr1, expr2, nullptr, yylineno);

                                        auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);

                                        n_expr->true_list.push_back(not_equal_quad->label);
                                        n_expr->false_list.push_back(jump_quad->label);

                                        $$ = n_expr;

                                        DLOG("expr -> expr != expr");
                                    }
            | expr AND              {
                                        auto tmp_expr1 = $1;
                                        if (tmp_expr1->get_type() != BOOL) {
                                            BoolExpr* expr1 = new BoolExpr(tmp_expr1, nullptr, nullptr);

                                            auto equal_quad = Emit(IF_EQ_t, tmp_expr1, new BoolConstant(true), nullptr, yylineno);
                                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);

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

                                            auto equal_quad = Emit(IF_EQ_t, $5, new BoolConstant(true), nullptr, yylineno);
                                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);

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

                                            auto equal_quad = Emit(IF_EQ_t, tmp_expr1, new BoolConstant(true), nullptr, yylineno);
                                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);

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

                                            auto equal_quad = Emit(IF_EQ_t, $5, new BoolConstant(true), nullptr, yylineno);
                                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);

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
                                        if (IsValidArithmeticOp(symbol, yylineno)) {
                                            auto temp = NewTemp(VAR, nullptr);
                                            Emit(UMINUS_t, temp, symbol, nullptr, yylineno);
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

                                        auto equal_quad = Emit(IF_EQ_t, expr1, new BoolConstant(true),  nullptr, yylineno);

                                        auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                        
                                        BOOL_EXPR_CAST(expr1)->true_list.push_back(equal_quad->label);
                                        BOOL_EXPR_CAST(expr1)->false_list.push_back(jump_quad->label);

                                        n_expr->true_list = BOOL_EXPR_CAST(expr1)->false_list;
                                        n_expr->false_list = BOOL_EXPR_CAST(expr1)->true_list;

                                        $$ = n_expr;

                                        DLOG("term -> not expr");
                                    }
            | PLUSPLUS lvalue       {
                                        auto symbol = $2;
                                        Symbol* result = NewTemp(VAR, nullptr);
                                        if (IsValidArithmeticOp(symbol, yylineno)) {
                                            if (IsTableItem(symbol)) {
                                                result = EmitIfTableItem(symbol, yylineno);
                                                Emit(ADD_t, result, result, new IntConstant(1), yylineno);
                                                Emit(TABLESETELEM_t, symbol, symbol->get_index(), result, yylineno);
                                            } else {
                                                Emit(ADD_t, symbol, symbol, new IntConstant(1), yylineno);
                                                Emit(ASSIGN_t, result, symbol, nullptr, yylineno);  
                                            }
                                        } 
                                        $$ = result;     
                                        DLOG("term -> ++lvalue"); 
                                    }
            | lvalue PLUSPLUS       {
                                        auto symbol = $1;
                                        auto result = NewTemp(VAR, nullptr);
                                        if (IsValidArithmeticOp(symbol, yylineno)) {
                                            if (IsTableItem(symbol)) {
                                                auto val = EmitIfTableItem(symbol, yylineno);
                                                Emit(ASSIGN_t, result, val, nullptr, yylineno);
                                                Emit(ADD_t, val, val, new IntConstant(1), yylineno);
                                                Emit(TABLESETELEM_t, symbol, symbol->get_index(), val, yylineno);
                                            } else {
                                                Emit(ASSIGN_t, result, symbol, nullptr, yylineno);    
                                                Emit(ADD_t, symbol, symbol, new IntConstant(1), yylineno);
                                            }
                                        } 
                                        $$ = result;
                                        DLOG("term -> lvalue++"); }
            | MINUSMINUS lvalue     { 
                                        auto symbol = $2;
                                        Symbol* result = NewTemp(VAR, nullptr);
                                        if (IsValidArithmeticOp(symbol, yylineno)) {
                                            if (IsTableItem(symbol)) {
                                                result = EmitIfTableItem(symbol, yylineno);
                                                Emit(SUB_t, result, result, new IntConstant(1), yylineno);
                                                Emit(TABLESETELEM_t, symbol, symbol->get_index(), result, yylineno);
                                            } else {
                                                Emit(SUB_t, symbol, symbol, new IntConstant(1), yylineno);
                                                Emit(ASSIGN_t, result, symbol, nullptr, yylineno);  
                                            }
                                        } 
                                        $$ = result;      
                                        DLOG("term -> --lvaule");
                                    }
            | lvalue MINUSMINUS     { 
                                        auto symbol = $1;
                                        auto result = NewTemp(VAR, nullptr);
                                        if (IsValidArithmeticOp(symbol, yylineno)) {
                                            if (IsTableItem(symbol)) {
                                                auto val = EmitIfTableItem(symbol, yylineno);
                                                Emit(ASSIGN_t, result, val, nullptr, yylineno);
                                                Emit(SUB_t, val, val, new IntConstant(1), yylineno);
                                                Emit(TABLESETELEM_t, symbol, symbol->get_index(), val, yylineno);
                                            } else {
                                                Emit(ASSIGN_t, result, symbol, nullptr, yylineno);    
                                                Emit(SUB_t, symbol, symbol, new IntConstant(1), yylineno);
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
                                            expr = ConcludeShortCircuit(BOOL_EXPR_CAST($3), yylineno);
                                        }
                                        if (IsValidAssign(symbol, yylineno)) {
                                            if (IsTableItem(symbol)) {
                                                Emit(TABLESETELEM_t, symbol, symbol->get_index(), expr, yylineno);
                                                auto result = EmitIfTableItem(symbol, yylineno);
                                                $$ = new AssignExpr(result, symbol, expr);
                                            } else {
                                                auto result = NewTemp(VAR, nullptr);
                                                Emit(ASSIGN_t, symbol, expr, nullptr, yylineno);
                                                Emit(ASSIGN_t, result, symbol, nullptr, yylineno);
                                                $$ = new AssignExpr(result, symbol, expr);
                                            }
                                        }
                                        DLOG("assignexpr -> lvalue = expr");
                                    }
            ;

primary:      lvalue                {
                                        $$ = EmitIfTableItem($1, yylineno);

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
                                        auto symbol = Lookup($1);
                                        if (symbol == nullptr) {
                                            symbol = DefineNewSymbol(VAR, $1, nullptr, yylineno);
                                        } else if (!symbol->is_active()) {
                                            SignalError("Cannot access " + symbol->get_id() + ", previously defined in line: " + std::to_string(symbol->get_line()), yylineno);
                                        }
                                        $$ = symbol;

                                        DLOG("lvalue -> id");
                                    }
            | LOCAL ID              {
                                        auto symbol = Lookup($2);
                                        if (symbol == nullptr) { 
                                            symbol = DefineNewSymbol(VAR, $2, nullptr, yylineno);
                                        }
                                        else if (!symbol->is_active()) {
                                            SignalError("Cannot access " + symbol->get_id() + ", previously defined in line: " + std::to_string(symbol->get_line()), yylineno);
                                        }
                                        else if (IsGlobalVar(symbol)) {
                                            symbol = DefineNewSymbol(VAR, $2, nullptr, yylineno);
                                        }
                                        else if (IsUserFunction(symbol)){
                                            if(IsAtCurrentScope(symbol)) 
                                                SignalError("Attempting to redefine a previously declared user function", yylineno);
                                            else 
                                                symbol = DefineNewSymbol(VAR, $2, nullptr, yylineno);
                                        }
                                        else if (IsLibraryFunction(symbol)) {
                                            SignalError("Attempting to redefine a library function", yylineno);
                                        }
                                        $$ = symbol;
                                        DLOG("lvalue -> local id");
                                    }
            | COLONCOLON ID         {
                                        auto symbol = LookupGlobal($2);
                                        if (symbol == nullptr || !symbol->is_active()) 
                                            SignalError("No global variable with id: " + std::string($2), yylineno);
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
                                        $$ = MemberItem(item, index, yylineno);
                                        DLOG("member -> lvalue.id");
                                    }
            | lvalue '[' expr ']'   {
                                        auto item = $1;
                                        auto index = $3;
                                        auto sym = EmitIfTableItem(item, yylineno);
                                        $$ = DefineNewSymbol(TABLE_ITEM, sym->get_id().c_str(), index, yylineno);
                                        DLOG("member -> lvalue[expr]");
                                    }
            | call '.' ID           {
                                        auto item = ($1)->get_ret_val();
                                        auto index = $3;
                                        $$ = MemberItem(item, index, yylineno);
                                        DLOG("member -> call.id");
                                    }
            | call '[' expr ']'     {
                                        auto item = ($1)->get_ret_val();
                                        auto index = $3;
                                        auto sym = EmitIfTableItem(item, yylineno);
                                        $$ = DefineNewSymbol(TABLE_ITEM, sym->get_id().c_str(), index, yylineno);
                                        DLOG("member -> call[expr]");
                                    }
            ;

call:       call  '(' elist ')'             {
                                                auto called_symbol = $1->get_ret_val();
                                                $$ = MakeCall(called_symbol, new NormCall($3), yylineno);
                                                DLOG("call -> call(elist)");
                                            }
            | lvalue  callsuffix            {
                                                auto called_symbol = $1;
                                                auto call_suffix = $2;
                                                called_symbol = EmitIfTableItem(called_symbol, yylineno);
                                                if (IsMethodCall(call_suffix)) {
                                                    auto t = called_symbol;
                                                    called_symbol = EmitIfTableItem(MemberItem(t, call_suffix->get_name().c_str(), yylineno), yylineno);
                                                    // called_symbol = MemberItem(t, call_suffix->get_name().c_str()); //??
                                                    call_suffix->InsertArg(t);
                                                }
                                                $$ = MakeCall(called_symbol, call_suffix, yylineno);
                                            }
            | '(' funcdef ')'  
                '(' elist ')'               {
                                                auto called_symbol = $2->get_sym();
                                                $$ = MakeCall(called_symbol, new NormCall($5), yylineno); 
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
                                        $2 = ConcludeShortCircuit(BOOL_EXPR_CAST($2), yylineno);
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
                                    $1 = ConcludeShortCircuit(BOOL_EXPR_CAST($1), yylineno);
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
                                    Emit(TABLECREATE_t, result, nullptr, nullptr, yylineno);

                                    auto elements = $2->exprs;
                                    unsigned int elem_cnt = 0;
                                    elements.reverse();
                                    for (auto element : elements)
                                        Emit(TABLESETELEM_t, result, new IntConstant(elem_cnt++), element, yylineno);
                                    $$ = new TableMakeElems(result, $2);    
                                    DLOG("objectdef -> [elist]");
                                }
            | '[' indexed ']'   {
                                    auto result = NewTemp(VAR, nullptr);
                                    Emit(TABLECREATE_t, result, nullptr, nullptr, yylineno);
                                    auto indexedelems = $2->pairs;
                                    indexedelems.reverse();
                                    for (auto indexedelem : indexedelems) 
                                        Emit(TABLESETELEM_t, result, indexedelem->pair.first, indexedelem->pair.second, yylineno);
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
                                            $4 = ConcludeShortCircuit(BOOL_EXPR_CAST($4), yylineno);
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

funcid:     ID              {
                                auto symbol = Lookup($1);
                                if (symbol == nullptr) {
                                    symbol = DefineNewSymbol(USER_FUNC, $1, nullptr, yylineno);
                                }
                                else if (!symbol->is_active()) {
                                    SignalError("Cannot access " + symbol->get_id() + ", previously defined in line: " + std::to_string(symbol->get_line()), yylineno);
                                }
                                else {
                                    if (IsVariable(symbol)) {
                                        SignalError(std::string($1) + " variable, previously defined in line: " + std::to_string(symbol->get_line()) + ", cannot be redefined as a function", yylineno);
                                    }
                                    else if (IsLibraryFunction(symbol)) {
                                        SignalError(std::string($1) + " library function cannot be shadowed by a user function", yylineno);
                                    }
                                    else if (IsAtCurrentScope(symbol)) {
                                        SignalError("Name collision with function " + std::string($1) + ", previously defined in line: " + std::to_string(symbol->get_line()), yylineno);
                                    }
                                    else{
                                        symbol = DefineNewSymbol(USER_FUNC, $1, nullptr, yylineno);  // shadow user function
                                    }
                                }
                                $$ = symbol;
                            }
            |               {
                                auto anonymous_function = DefineNewAnonymousFunc(yylineno);
                                $$ = anonymous_function;
                            }
            ;

funcprefix: FUNCTION funcid {
                                auto func_def_stmt = new FuncDefStmt($2);

                                func_def_stmts.push(func_def_stmt);

                                auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                ($2)->set_taddress(NextQuadLabel());
                                Emit(FUNCSTART_t, $2, nullptr, nullptr, yylineno);

                                func_def_stmt->set_func_start_jump_quad(jump_quad);

                                $$ = $2;
                                
                                StoreFuncLocalOffset();
                                EnterScopeSpace();
                                ResetFormalArgOffset();
                            }

funcargs:   '(' idlist ')'  {
                                EnterScopeSpace();
                                ResetFuncLocalOffset();
                                HideLowerScopes();
                                stmt_stack.push_back(FUNC_t);
                            }

funcbody:   block           {
                                $$ = CurrScopeOffset();
                                ExitScopeSpace();
                            }

funcdef:    funcprefix
            funcargs
            funcbody       {
                                ExitScopeSpace();
 
                                auto top_func_def = func_def_stmts.top();
                                auto function = $1;
                                function->set_total_local($3);
                                auto func_end_quad = Emit(FUNCEND_t, function, nullptr, nullptr, yylineno);

                                top_func_def->PatchFuncStartJumpQuad(func_end_quad->label + 1);
                                top_func_def->PatchReturnJumpQuads(func_end_quad->label);

                                func_def_stmts.pop();

                                EnableLowerScopes();

                                stmt_stack.pop_back();

                                RestoreFuncLocalOffset();

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
                        auto offset = CurrScopeOffset();
                        
                        IncreaseCurrOffset();

                        auto new_formal_arg = new Symbol(VAR, $2, yylineno, CurrScope() + 1, FORMAL_ARG, offset, nullptr);

                        if (func->HasFormalArg(new_formal_arg)) {
                            SignalError("formal argument " + std::string($2) + " already declared", yylineno);
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
                        auto offset = CurrScopeOffset();
                        IncreaseCurrOffset();

                        auto new_formal_arg = new Symbol(VAR, $1, yylineno, CurrScope() + 1, FORMAL_ARG, offset, nullptr);
                        if (func->HasFormalArg(new_formal_arg)) {
                            SignalError("formal argument " + std::string($1) + " already declared", yylineno);
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
                                                    $3 = ConcludeShortCircuit(BOOL_EXPR_CAST($3), yylineno);
                                                }

                                                auto if_stmt = new IfStmt();
                                                if_stmts.push(if_stmt);

                                                auto branch_quad = Emit(IF_EQ_t, $3, new BoolConstant(true), nullptr, yylineno);
                                                auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno); 

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

                                auto else_jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);

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

                                top_if_stmt->PatchIfJumpQuad(NextQuadLabel());

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
                                        $4 = ConcludeShortCircuit(BOOL_EXPR_CAST($4), yylineno);
                                    }

                                    auto top_while_stmt = while_stmts.top();

                                    auto branch_quad = Emit(IF_EQ_t, $4, new BoolConstant(true), nullptr, yylineno);
                                    auto exit_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);

                                    top_while_stmt->PushLoopQuad(branch_quad);
                                    top_while_stmt->PushLoopQuad(exit_quad);

                                    stmt_stack.push_back(LOOP_t);
                                }
            stmt                { 
                                    auto top_while_stmt = while_stmts.top();

                                    auto loop_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);

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
                                                            $7 = ConcludeShortCircuit(BOOL_EXPR_CAST($7), yylineno);
                                                        }

                                                        auto top_for_stmt = for_stmts.top();

                                                        auto branch_quad = Emit(IF_EQ_t, $7, new BoolConstant(true), nullptr, yylineno);

                                                        top_for_stmt->PushLoopQuad(branch_quad);

                                                        auto exit_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);

                                                        top_for_stmt->PushLoopQuad(exit_quad);

                                                        auto exprs_first_quad_label = NextQuadLabel();

                                                        top_for_stmt->set_exprs_first_quad_label(exprs_first_quad_label);
                                                    }
            elist ')'                               {
                                                        auto top_for_stmt = for_stmts.top();
                                                        auto loop_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                                        top_for_stmt->PushLoopQuad(loop_quad);
                                                        stmt_stack.push_back(LOOP_t);
                                                    }
            stmt                                    {
                                                        auto top_for_stmt = for_stmts.top();
                                                        auto expr_jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno); 

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
                                SignalError("Invalid return, used outside a function block", yylineno);
                            } else {
                                auto top_func_def = func_def_stmts.top();
                                Emit(RET_t, nullptr, nullptr, nullptr, yylineno);
                                auto return_jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);

                                top_func_def->PushReturnJumpQuad(return_jump_quad);
                            }
                        } 
            ';'         {
                            DLOG("returnstmt -> RETURN;"); 
                        }
            | RETURN    
            expr ';'    {
                            if ($2->get_type() == BOOL) {
                                $2 = ConcludeShortCircuit(BOOL_EXPR_CAST($2), yylineno);
                            }

                            if (!InFuncDef()) 
                                SignalError("Invalid return, used outside a function block", yylineno);
                            else {
                                auto top_func_def = func_def_stmts.top();
                                Emit(RET_t, $2, nullptr, nullptr, yylineno);
                                auto return_jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);

                                top_func_def->PushReturnJumpQuad(return_jump_quad);
                                DLOG("returnstmt -> RETURN expr;");
                            }    
                        }
            ;

%%

int yyerror(std::string yaccProvidedMessage) {
    SignalError(yaccProvidedMessage, yylineno);
    return 1;
}

#ifndef TESTING 
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
            const char *path="../../quads.txt";
            std::ofstream quad_file(path);
            LogQuads(quad_file);
            quad_file.close();
        }
    #endif  
    opcode_dispatcher.Generate();
    if (NoErrorSignaled())
        make_binary_file();
    #if defined LOGINSTRS
        if (NoErrorSignaled()) { 
            LogInstructions(std::cout);
        }
    #endif
    #if defined LOGPCONSTS
        if (NoErrorSignaled()) { 
            const char *path = "../../program_consts.txt";
            std::ofstream pconsts_file(path);
            LogProgramConsts(pconsts_file);
            pconsts_file.close();
        }
    #endif
    return 0;
}
#endif