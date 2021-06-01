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

    #define BOOL_EXPR_CAST(e)   static_cast<expression::BoolExpr*>(e)

    union bison_begone{
        class intermediate_code::FuncDefStmt*   funcdef;

        class expression::Expression*           expr;
        class expression::AssignExpr*           assignexpr;

        class expression::Primary*              prim;
        class expression::Constant*             con;
        class expression::Call*                 call;
        class expression::CallSuffix*           call_suffix;
        class expression::NormCall*             norm_call;
        class expression::MethodCall*           method_call;
        class expression::Elist*                elist;
        class expression::Symbol*               sym;
        class expression::TableMake*            tablemake;
        class expression::Indexed*              indexed;
        class expression::IndexedElem*          indexed_elem; 
    };
    
%}

%union {                                                    
    union bison_begone*                     bypass;
    char*                                   stringValue;
    int                                     intValue;
    double                                  doubleValue;

    unsigned int                            quad_label;
}

%start program

%token EQUAL NOTEQUAL COLONCOLON DOTDOT GEQL LEQL MINUSMINUS PLUSPLUS
%token LOCAL FUNCTION IF ELSE FOR WHILE BREAK CONTINUE NIL TRUE FALSE RETURN

%token <stringValue>    STRING ID 
%token <intValue>       INTNUM
%token <doubleValue>    DOUBLENUM

%type <quad_label>      M funcbody

%type <bypass->funcdef>         funcdef

%type <bypass->expr>            term expr

%type <bypass->assignexpr>      assignexpr

%type <bypass->prim>            primary
%type <bypass->con>             const
%type <bypass->sym>             lvalue member funcid funcprefix
%type <bypass->call>            call
%type <bypass->call_suffix>     callsuffix
%type <bypass->norm_call>       normcall
%type <bypass->method_call>     methodcall
%type <bypass->elist>           elist multelist
%type <bypass->tablemake>       objectdef
%type <bypass->indexed>         indexed multindexed
%type <bypass->indexed_elem>    indexedelem

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
                                        if ($1->get_type() == expression::BOOL) {
                                            intermediate_code::ConcludeShortCircuit(BOOL_EXPR_CAST($1), yylineno);
                                        }
                                        intermediate_code::ResetTemp();
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
                                        if (!intermediate_code::IsValidBreakContinue()) {
                                            syntax_analysis::SignalError("invalid keyword BREAK outside of loop", yylineno);
                                        } else {
                                            auto jump_quad = intermediate_code::Emit(intermediate_code::JUMP_t, nullptr, nullptr, nullptr, yylineno);

                                            auto top_loop_stmt = intermediate_code::loop_stmts.top();
                                            top_loop_stmt->PushBreakJumpQuad(jump_quad);
                                        }

                                        DLOG("stmt -> break;");
                                    }
            | CONTINUE ';'          {
                                        if (!intermediate_code::IsValidBreakContinue()) {
                                            syntax_analysis::SignalError("invalid keyword CONTINUE outside of loop", yylineno);
                                        } else {
                                            auto jump_quad = intermediate_code::Emit(intermediate_code::JUMP_t, nullptr, nullptr, nullptr, yylineno);

                                            auto top_loop_stmt = intermediate_code::loop_stmts.top();
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
                                        if (intermediate_code::IsValidArithmeticOp(expr1, yylineno) & intermediate_code::IsValidArithmeticOp(expr2, yylineno)) {
                                            auto temp = intermediate_code::NewTemp(expression::VAR, nullptr);
                                            intermediate_code::Emit(intermediate_code::ADD_t, temp, expr1, expr2, yylineno);
                                            $$ = new expression::ArithmeticExpr(temp, expr1, expr2);
                                        }
                                        DLOG("expr -> expr + expr");
                                    }
            | expr '-' expr         {
                                        auto expr1 = $1;
                                        auto expr2 = $3;
                                        if (intermediate_code::IsValidArithmeticOp(expr1, yylineno) & intermediate_code::IsValidArithmeticOp(expr2, yylineno)) {
                                            auto temp = intermediate_code::NewTemp(expression::VAR, nullptr);
                                            intermediate_code::Emit(intermediate_code::SUB_t, temp, expr1, expr2, yylineno);
                                            $$ = new expression::ArithmeticExpr(temp, expr1, expr2);
                                        } 
                                        DLOG("expr -> expr - expr");
                                    }
            | expr '*' expr         {
                                        auto expr1 = $1;
                                        auto expr2 = $3;
                                        if (intermediate_code::IsValidArithmeticOp(expr1, yylineno) & intermediate_code::IsValidArithmeticOp(expr2, yylineno)) {
                                            auto temp = intermediate_code::NewTemp(expression::VAR, nullptr);
                                            intermediate_code::Emit(intermediate_code::MUL_t, temp, expr1, expr2, yylineno);
                                            $$ = new expression::ArithmeticExpr(temp, expr1, expr2);
                                        }
                                        DLOG("expr -> expr * expr");
                                    }
            | expr '/' expr         {
                                        auto expr1 = $1;
                                        auto expr2 = $3;
                                        if (intermediate_code::IsValidArithmeticOp(expr1, yylineno) & intermediate_code::IsValidArithmeticOp(expr2, yylineno)) {
                                            auto temp = intermediate_code::NewTemp(expression::VAR, nullptr);
                                            intermediate_code::Emit(intermediate_code::DIV_t, temp, expr1, expr2, yylineno);
                                            $$ = new expression::ArithmeticExpr(temp, expr1, expr2);
                                        }
                                        DLOG("expr -> expr / expr");
                                    }
            | expr '%' expr         {
                                        auto expr1 = $1;
                                        auto expr2 = $3;
                                        if (intermediate_code::IsValidArithmeticOp(expr1, yylineno) & intermediate_code::IsValidArithmeticOp(expr2, yylineno)) {
                                            auto temp = intermediate_code::NewTemp(expression::VAR, nullptr);
                                            intermediate_code::Emit(intermediate_code::MOD_t, temp, expr1, expr2, yylineno);
                                            $$ = new expression::ArithmeticExpr(temp, expr1, expr2);
                                        } 
                                        DLOG("expr -> expr % expr");
                                    }
            | expr '>' expr         {
                                        auto expr1 = $1;
                                        auto expr2 = $3;
                                        if (intermediate_code::IsValidArithmeticComp(expr1, yylineno) & intermediate_code::IsValidArithmeticComp(expr2, yylineno)) {
                                            expression::BoolExpr* n_expr = new expression::BoolExpr(expr1, expr2, nullptr);

                                            auto greater_quad = intermediate_code::Emit(intermediate_code::IF_GREATER_t, expr1, expr2, nullptr, yylineno);

                                            auto jump_quad = intermediate_code::Emit(intermediate_code::JUMP_t, nullptr, nullptr, nullptr, yylineno);

                                            n_expr->true_list.push_back(greater_quad->label);
                                            n_expr->false_list.push_back(jump_quad->label);

                                            $$ = n_expr;
                                        }
                                        DLOG("expr -> expr > expr");
                                    }
            | expr GEQL expr        {
                                        auto expr1 = $1;
                                        auto expr2 = $3;
                                        if (intermediate_code::IsValidArithmeticComp(expr1, yylineno) & intermediate_code::IsValidArithmeticComp(expr2, yylineno)) {
                                            expression::BoolExpr* n_expr = new expression::BoolExpr(expr1, expr2, nullptr);

                                            auto greater_equal_quad = intermediate_code::Emit(intermediate_code::IF_GREATEREQ_t, expr1, expr2, nullptr, yylineno);

                                            auto jump_quad = intermediate_code::Emit(intermediate_code::JUMP_t, nullptr, nullptr, nullptr, yylineno);

                                            n_expr->true_list.push_back(greater_equal_quad->label);
                                            n_expr->false_list.push_back(jump_quad->label);

                                            $$ = n_expr;
                                        }

                                        DLOG("expr -> expr >= expr");
                                    }
            | expr '<' expr         {
                                        auto expr1 = $1;
                                        auto expr2 = $3;
                                        if (intermediate_code::IsValidArithmeticComp(expr1, yylineno) & intermediate_code::IsValidArithmeticComp(expr2, yylineno)) {
                                            expression::BoolExpr* n_expr = new expression::BoolExpr(expr1, expr2, nullptr);

                                            auto less_quad = intermediate_code::Emit(intermediate_code::IF_LESS_t, expr1, expr2, nullptr, yylineno);

                                            auto jump_quad = intermediate_code::Emit(intermediate_code::JUMP_t, nullptr, nullptr, nullptr, yylineno);

                                            n_expr->true_list.push_back(less_quad->label);
                                            n_expr->false_list.push_back(jump_quad->label);

                                            $$ = n_expr;
                                        }
                                        DLOG("expr -> expr + expr");
                                    }
            | expr LEQL expr        {
                                        auto expr1 = $1;
                                        auto expr2 = $3;
                                        if (intermediate_code::IsValidArithmeticComp(expr1, yylineno) & intermediate_code::IsValidArithmeticComp(expr2, yylineno)) {
                                            expression::BoolExpr* n_expr = new expression::BoolExpr(expr1, expr2, nullptr);

                                            auto less_equal_quad = intermediate_code::Emit(intermediate_code::IF_LESSEQ_t, expr1, expr2, nullptr, yylineno);

                                            auto jump_quad = intermediate_code::Emit(intermediate_code::JUMP_t, nullptr, nullptr, nullptr, yylineno);

                                            n_expr->true_list.push_back(less_equal_quad->label);
                                            n_expr->false_list.push_back(jump_quad->label);

                                            $$ = n_expr;
                                        }
                                        DLOG("expr -> expr <= expr");
                                    }
            | expr EQUAL            {
                                        if ($1->get_type() == expression::BOOL) {
                                            $1 = intermediate_code::ConcludeShortCircuit(BOOL_EXPR_CAST($1), yylineno);
                                        }
                                    }
            expr                    {
                                        if ($4->get_type() == expression::BOOL) {
                                            $4 = intermediate_code::ConcludeShortCircuit(BOOL_EXPR_CAST($4), yylineno);
                                        }

                                        auto expr1 = $1;
                                        auto expr2 = $4;

                                        expression::BoolExpr* n_expr = new expression::BoolExpr(expr1, expr2, nullptr);

                                        auto equal_quad = intermediate_code::Emit(intermediate_code::IF_EQ_t, expr1, expr2, nullptr, yylineno);

                                        auto jump_quad = intermediate_code::Emit(intermediate_code::JUMP_t, nullptr, nullptr, nullptr, yylineno);

                                        n_expr->true_list.push_back(equal_quad->label);
                                        n_expr->false_list.push_back(jump_quad->label);

                                        $$ = n_expr;

                                        DLOG("expr -> expr == expr");
                                    }
            | expr NOTEQUAL         {
                                        if ($1->get_type() == expression::BOOL) {
                                            $1 = intermediate_code::ConcludeShortCircuit(BOOL_EXPR_CAST($1), yylineno);
                                        }
                                    }
            expr                    {
                                        if ($4->get_type() == expression::BOOL) {
                                            $4 = intermediate_code::ConcludeShortCircuit(BOOL_EXPR_CAST($4), yylineno);
                                        }

                                        auto expr1 = $1;
                                        auto expr2 = $4;

                                        expression::BoolExpr* n_expr = new expression::BoolExpr(expr1, expr2, nullptr);

                                        auto not_equal_quad = intermediate_code::Emit(intermediate_code::IF_NOTEQ_t,  expr1, expr2, nullptr, yylineno);

                                        auto jump_quad = intermediate_code::Emit(intermediate_code::JUMP_t, nullptr, nullptr, nullptr, yylineno);

                                        n_expr->true_list.push_back(not_equal_quad->label);
                                        n_expr->false_list.push_back(jump_quad->label);

                                        $$ = n_expr;

                                        DLOG("expr -> expr != expr");
                                    }
            | expr AND              {
                                        auto tmp_expr1 = $1;
                                        if (tmp_expr1->get_type() != expression::BOOL) {
                                            expression::BoolExpr* expr1 = new expression::BoolExpr(tmp_expr1, nullptr, nullptr);

                                            auto equal_quad = intermediate_code::Emit(intermediate_code::IF_EQ_t, tmp_expr1, new expression::BoolConstant(true), nullptr, yylineno);
                                            auto jump_quad = intermediate_code::Emit(intermediate_code::JUMP_t, nullptr, nullptr, nullptr, yylineno);

                                            expr1->true_list.push_back(equal_quad->label);
                                            expr1->false_list.push_back(jump_quad->label);

                                            $1 = expr1;
                                        }
                                    }
            M expr                  {
                                        auto expr1 = $1;
                                        auto expr2 = $5;
                                        auto q_label = $4;

                                        if (expr2->get_type() != expression::BOOL) {
                                            expr2 = new expression::BoolExpr($5, nullptr, nullptr);

                                            auto equal_quad = intermediate_code::Emit(intermediate_code::IF_EQ_t, $5, new expression::BoolConstant(true), nullptr, yylineno);
                                            auto jump_quad = intermediate_code::Emit(intermediate_code::JUMP_t, nullptr, nullptr, nullptr, yylineno);

                                            BOOL_EXPR_CAST(expr2)->true_list.push_back(equal_quad->label);
                                            BOOL_EXPR_CAST(expr2)->false_list.push_back(jump_quad->label);
                                        }

                                        expression::BoolExpr* n_expr = new expression::BoolExpr(expr1, expr2, nullptr);

                                        intermediate_code::BackPatch(BOOL_EXPR_CAST(expr1)->true_list, q_label);
                                        n_expr->true_list = BOOL_EXPR_CAST(expr2)->true_list;
                                        n_expr->false_list = BOOL_EXPR_CAST(expr1)->false_list;
                                        n_expr->false_list.merge(BOOL_EXPR_CAST(expr2)->false_list);

                                        $$ = n_expr;

                                        DLOG("expr -> expr and expr");
                                    }
            | expr OR               {
                                        auto tmp_expr1 = $1;
                                        if (tmp_expr1->get_type() != expression::BOOL) {
                                            expression::BoolExpr* expr1 = new expression::BoolExpr(tmp_expr1, nullptr, nullptr);

                                            auto equal_quad = intermediate_code::Emit(intermediate_code::IF_EQ_t, tmp_expr1, new expression::BoolConstant(true), nullptr, yylineno);
                                            auto jump_quad = intermediate_code::Emit(intermediate_code::JUMP_t, nullptr, nullptr, nullptr, yylineno);

                                            expr1->true_list.push_back(equal_quad->label);
                                            expr1->false_list.push_back(jump_quad->label);

                                            $1 = expr1;
                                        }
                                    }
            M expr                  {
                                        auto expr1 = $1;
                                        auto expr2 = $5;
                                        auto q_label = $4;

                                        if (expr2->get_type() != expression::BOOL) {
                                            expr2 = new expression::BoolExpr($5, nullptr, nullptr);

                                            auto equal_quad = intermediate_code::Emit(intermediate_code::IF_EQ_t, $5, new expression::BoolConstant(true), nullptr, yylineno);
                                            auto jump_quad = intermediate_code::Emit(intermediate_code::JUMP_t, nullptr, nullptr, nullptr, yylineno);

                                            BOOL_EXPR_CAST(expr2)->true_list.push_back(equal_quad->label);
                                            BOOL_EXPR_CAST(expr2)->false_list.push_back(jump_quad->label);
                                        }

                                        expression::BoolExpr* n_expr = new expression::BoolExpr(expr1, expr2, nullptr);

                                        intermediate_code::BackPatch(BOOL_EXPR_CAST(expr1)->false_list, q_label);
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
                                        $<quad_label>$ = intermediate_code::NextQuadLabel();
                                    }
            ;

term:         '(' expr ')'          {
                                        $$ = $2;
                                        DLOG("term -> (expr)");
                                    }
            | '-' expr %prec UMINUS {
                                        auto symbol = $2;
                                        if (intermediate_code::IsValidArithmeticOp(symbol, yylineno)) {
                                            auto temp = intermediate_code::NewTemp(expression::VAR, nullptr);
                                            intermediate_code::Emit(intermediate_code::UMINUS_t, temp, symbol, nullptr, yylineno);
                                            $$ = symbol;
                                        }
                                        DLOG("term -> -expr");
                                    }
            | NOT expr              {
                                        auto expr1 = $2;

                                        if (expr1->get_type() != expression::BOOL) {
                                            expr1 = new expression::BoolExpr($2, nullptr, nullptr);
                                        }

                                        expression::BoolExpr* n_expr = new expression::BoolExpr(expr1, nullptr, nullptr);

                                        auto equal_quad = intermediate_code::Emit(intermediate_code::IF_EQ_t, expr1, new expression::BoolConstant(true),  nullptr, yylineno);

                                        auto jump_quad = intermediate_code::Emit(intermediate_code::JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                        
                                        BOOL_EXPR_CAST(expr1)->true_list.push_back(equal_quad->label);
                                        BOOL_EXPR_CAST(expr1)->false_list.push_back(jump_quad->label);

                                        n_expr->true_list = BOOL_EXPR_CAST(expr1)->false_list;
                                        n_expr->false_list = BOOL_EXPR_CAST(expr1)->true_list;

                                        $$ = n_expr;

                                        DLOG("term -> not expr");
                                    }
            | PLUSPLUS lvalue       {
                                        auto symbol = $2;
                                        expression::Symbol* result = intermediate_code::NewTemp(expression::VAR, nullptr);
                                        if (intermediate_code::IsValidArithmeticOp(symbol, yylineno)) {
                                            if (expression::IsTableItem(symbol)) {
                                                result = intermediate_code::EmitIfTableItem(symbol, yylineno);
                                                intermediate_code::Emit(intermediate_code::ADD_t, result, result, new expression::IntConstant(1), yylineno);
                                                intermediate_code::Emit(intermediate_code::TABLESETELEM_t, symbol, symbol->get_index(), result, yylineno);
                                            } else {
                                                intermediate_code::Emit(intermediate_code::ADD_t, symbol, symbol, new expression::IntConstant(1), yylineno);
                                                intermediate_code::Emit(intermediate_code::ASSIGN_t, result, symbol, nullptr, yylineno);  
                                            }
                                        } 
                                        $$ = result;     
                                        DLOG("term -> ++lvalue"); 
                                    }
            | lvalue PLUSPLUS       {
                                        auto symbol = $1;
                                        auto result = intermediate_code::NewTemp(expression::VAR, nullptr);
                                        if (intermediate_code::IsValidArithmeticOp(symbol, yylineno)) {
                                            if (expression::IsTableItem(symbol)) {
                                                auto val = intermediate_code::EmitIfTableItem(symbol, yylineno);
                                                intermediate_code::Emit(intermediate_code::ASSIGN_t, result, val, nullptr, yylineno);
                                                intermediate_code::Emit(intermediate_code::ADD_t, val, val, new expression::IntConstant(1), yylineno);
                                                intermediate_code::Emit(intermediate_code::TABLESETELEM_t, symbol, symbol->get_index(), val, yylineno);
                                            } else {
                                                intermediate_code::Emit(intermediate_code::ASSIGN_t, result, symbol, nullptr, yylineno);    
                                                intermediate_code::Emit(intermediate_code::ADD_t, symbol, symbol, new expression::IntConstant(1), yylineno);
                                            }
                                        } 
                                        $$ = result;
                                        DLOG("term -> lvalue++"); }
            | MINUSMINUS lvalue     { 
                                        auto symbol = $2;
                                        expression::Symbol* result = intermediate_code::NewTemp(expression::VAR, nullptr);
                                        if (intermediate_code::IsValidArithmeticOp(symbol, yylineno)) {
                                            if (expression::IsTableItem(symbol)) {
                                                result = intermediate_code::EmitIfTableItem(symbol, yylineno);
                                                intermediate_code::Emit(intermediate_code::SUB_t, result, result, new expression::IntConstant(1), yylineno);
                                                intermediate_code::Emit(intermediate_code::TABLESETELEM_t, symbol, symbol->get_index(), result, yylineno);
                                            } else {
                                                intermediate_code::Emit(intermediate_code::SUB_t, symbol, symbol, new expression::IntConstant(1), yylineno);
                                                intermediate_code::Emit(intermediate_code::ASSIGN_t, result, symbol, nullptr, yylineno);  
                                            }
                                        } 
                                        $$ = result;      
                                        DLOG("term -> --lvaule");
                                    }
            | lvalue MINUSMINUS     { 
                                        auto symbol = $1;
                                        auto result = intermediate_code::NewTemp(expression::VAR, nullptr);
                                        if (intermediate_code::IsValidArithmeticOp(symbol, yylineno)) {
                                            if (expression::IsTableItem(symbol)) {
                                                auto val = intermediate_code::EmitIfTableItem(symbol, yylineno);
                                                intermediate_code::Emit(intermediate_code::ASSIGN_t, result, val, nullptr, yylineno);
                                                intermediate_code::Emit(intermediate_code::SUB_t, val, val, new expression::IntConstant(1), yylineno);
                                                intermediate_code::Emit(intermediate_code::TABLESETELEM_t, symbol, symbol->get_index(), val, yylineno);
                                            } else {
                                                intermediate_code::Emit(intermediate_code::ASSIGN_t, result, symbol, nullptr, yylineno);    
                                                intermediate_code::Emit(intermediate_code::SUB_t, symbol, symbol, new expression::IntConstant(1), yylineno);
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
                                        if ($3->get_type() == expression::BOOL) {
                                            expr = intermediate_code::ConcludeShortCircuit(BOOL_EXPR_CAST($3), yylineno);
                                        }
                                        if (intermediate_code::IsValidAssign(symbol, yylineno)) {
                                            if (expression::IsTableItem(symbol)) {
                                                intermediate_code::Emit(intermediate_code::TABLESETELEM_t, symbol, symbol->get_index(), expr, yylineno);
                                                auto result = intermediate_code::EmitIfTableItem(symbol, yylineno);
                                                $$ = new expression::AssignExpr(result, symbol, expr);
                                            } else {
                                                auto result = intermediate_code::NewTemp(expression::VAR, nullptr);
                                                intermediate_code::Emit(intermediate_code::ASSIGN_t, symbol, expr, nullptr, yylineno);
                                                intermediate_code::Emit(intermediate_code::ASSIGN_t, result, symbol, nullptr, yylineno);
                                                $$ = new expression::AssignExpr(result, symbol, expr);
                                            }
                                        }
                                        DLOG("assignexpr -> lvalue = expr");
                                    }
            ;

primary:      lvalue                {
                                        $$ = intermediate_code::EmitIfTableItem($1, yylineno);

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
                                        auto symbol = syntax_analysis::Lookup($1);
                                        if (symbol == nullptr) {
                                            symbol = syntax_analysis::DefineNewSymbol(expression::VAR, $1, nullptr, yylineno);
                                        } else if (!symbol->is_active()) {
                                            syntax_analysis::SignalError("Cannot access " + symbol->get_id() + ", previously defined in line: " + std::to_string(symbol->get_line()), yylineno);
                                        }
                                        $$ = symbol;

                                        DLOG("lvalue -> id");
                                    }
            | LOCAL ID              {
                                        auto symbol = syntax_analysis::Lookup($2);
                                        if (symbol == nullptr) { 
                                            symbol = syntax_analysis::DefineNewSymbol(expression::VAR, $2, nullptr, yylineno);
                                        }
                                        else if (!symbol->is_active()) {
                                            syntax_analysis::SignalError("Cannot access " + symbol->get_id() + ", previously defined in line: " + std::to_string(symbol->get_line()), yylineno);
                                        }
                                        else if (syntax_analysis::IsGlobalVar(symbol)) {
                                            symbol = syntax_analysis::DefineNewSymbol(expression::VAR, $2, nullptr, yylineno);
                                        }
                                        else if (expression::IsUserFunction(symbol)){
                                            if(syntax_analysis::IsAtCurrentScope(symbol)) 
                                                syntax_analysis::SignalError("Attempting to redefine a previously declared user function", yylineno);
                                            else 
                                                symbol = syntax_analysis::DefineNewSymbol(expression::VAR, $2, nullptr, yylineno);
                                        }
                                        else if (expression::IsLibraryFunction(symbol)) {
                                            syntax_analysis::SignalError("Attempting to redefine a library function", yylineno);
                                        }
                                        $$ = symbol;
                                        DLOG("lvalue -> local id");
                                    }
            | COLONCOLON ID         {
                                        auto symbol = syntax_analysis::LookupGlobal($2);
                                        if (symbol == nullptr || !symbol->is_active()) 
                                            syntax_analysis::SignalError("No global variable with id: " + std::string($2), yylineno);
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
                                        $$ = intermediate_code::MemberItem(item, index, yylineno);
                                        DLOG("member -> lvalue.id");
                                    }
            | lvalue '[' expr ']'   {
                                        auto item = $1;
                                        auto index = $3;
                                        auto sym = intermediate_code::EmitIfTableItem(item, yylineno);
                                        $$ = syntax_analysis::DefineNewSymbol(expression::TABLE_ITEM, sym->get_id().c_str(), index, yylineno);
                                        DLOG("member -> lvalue[expr]");
                                    }
            | call '.' ID           {
                                        auto item = ($1)->get_ret_val();
                                        auto index = $3;
                                        $$ = intermediate_code::MemberItem(item, index, yylineno);
                                        DLOG("member -> call.id");
                                    }
            | call '[' expr ']'     {
                                        auto item = ($1)->get_ret_val();
                                        auto index = $3;
                                        auto sym = intermediate_code::EmitIfTableItem(item, yylineno);
                                        $$ = syntax_analysis::DefineNewSymbol(expression::TABLE_ITEM, sym->get_id().c_str(), index, yylineno);
                                        DLOG("member -> call[expr]");
                                    }
            ;

call:       call  '(' elist ')'             {
                                                auto called_symbol = $1->get_ret_val();
                                                $$ = intermediate_code::MakeCall(called_symbol, new expression::NormCall($3), yylineno);
                                                DLOG("call -> call(elist)");
                                            }
            | lvalue  callsuffix            {
                                                auto called_symbol = $1;
                                                auto call_suffix = $2;
                                                called_symbol = intermediate_code::EmitIfTableItem(called_symbol, yylineno);
                                                if (expression::IsMethodCall(call_suffix)) {
                                                    auto t = called_symbol;
                                                    called_symbol = intermediate_code::EmitIfTableItem(intermediate_code::MemberItem(t, call_suffix->get_name().c_str(), yylineno), yylineno);
                                                    // called_symbol = intermediate_code::MemberItem(t, call_suffix->get_name().c_str()); //??
                                                    call_suffix->InsertArg(t);
                                                }
                                                $$ = intermediate_code::MakeCall(called_symbol, call_suffix, yylineno);
                                            }
            | '(' funcdef ')'  
                '(' elist ')'               {
                                                auto called_symbol = $2->get_sym();
                                                $$ = intermediate_code::MakeCall(called_symbol, new expression::NormCall($5), yylineno); 
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
                                $$ = new expression::NormCall($2);
                                DLOG("normcall -> (elist)"); 
                            }
            ;

methodcall: DOTDOT ID '(' elist ')' {
                                        $$ = new expression::MethodCall(std::string($2), $4);
                                        DLOG("methodcall -> ..id(elist)");
                                    }
            ;

multelist:  ',' expr multelist  {
                                    if ($2->get_type() == expression::BOOL) {
                                        $2 = intermediate_code::ConcludeShortCircuit(BOOL_EXPR_CAST($2), yylineno);
                                    }
                                    expression::Elist* elist = new expression::Elist();
                                    elist->exprs.merge($3->exprs);
                                    elist->exprs.push_back($2);
                                    $$ = elist;
                                    DLOG("multelist -> ,expr multelist");
                                }
            |                   {
                                    $$ = new expression::Elist();
                                    DLOG("multelist -> EMPTY");
                                }
            ;

elist:      expr multelist  {
                                if ($1->get_type() == expression::BOOL) {
                                    $1 = intermediate_code::ConcludeShortCircuit(BOOL_EXPR_CAST($1), yylineno);
                                }
                                expression::Elist* elist = new expression::Elist();
                                elist->exprs.merge($2->exprs);
                                elist->exprs.push_back($1);
                                $$ = elist; 
                                DLOG("elist -> expr multelist");
                            }
            |               {
                                $$ = new expression::Elist();
                                DLOG("elist -> EMPTY");
                            }
            ;

objectdef:  '[' elist ']'       {
                                    auto result = intermediate_code::NewTemp(expression::VAR, nullptr);
                                    intermediate_code::Emit(intermediate_code::TABLECREATE_t, result, nullptr, nullptr, yylineno);

                                    auto elements = $2->exprs;
                                    unsigned int elem_cnt = 0;
                                    elements.reverse();
                                    for (auto element : elements)
                                        intermediate_code::Emit(intermediate_code::TABLESETELEM_t, result, new expression::IntConstant(elem_cnt++), element, yylineno);
                                    $$ = new expression::TableMakeElems(result, $2);    
                                    DLOG("objectdef -> [elist]");
                                }
            | '[' indexed ']'   {
                                    auto result = intermediate_code::NewTemp(expression::VAR, nullptr);
                                    intermediate_code::Emit(intermediate_code::TABLECREATE_t, result, nullptr, nullptr, yylineno);
                                    auto indexedelems = $2->pairs;
                                    indexedelems.reverse();
                                    for (auto indexedelem : indexedelems) 
                                        intermediate_code::Emit(intermediate_code::TABLESETELEM_t, result, indexedelem->pair.first, indexedelem->pair.second, yylineno);
                                    $$ = new expression::TableMakePairs(result, $2);    
                                    DLOG("objectdef -> [indexed]");
                                }
            ;

multindexed:',' indexedelem multindexed {
                                            expression::Indexed* indexed = new expression::Indexed();
                                            indexed->pairs.merge($3->pairs);
                                            indexed->pairs.push_back($2);
                                            $$ = indexed;
                                            DLOG("multindexed -> , indexedelem multidexed"); 
                                        }
            |                           {
                                            $$ = new expression::Indexed();
                                            DLOG("elsestmt -> EMPTY");
                                        }
            ;

indexed:    indexedelem multindexed {
                                        expression::Indexed* indexed = new expression::Indexed();
                                        indexed->pairs.merge($2->pairs);
                                        indexed->pairs.push_back($1);
                                        $$ = indexed; 
                                        DLOG("indexed -> indexedelem multidexed"); 
                                    }
            ;

indexedelem:'{' expr ':' expr '}'   {
                                        if ($4->get_type() == expression::BOOL) {
                                            $4 = intermediate_code::ConcludeShortCircuit(BOOL_EXPR_CAST($4), yylineno);
                                        }
                                        $$ = new expression::IndexedElem(std::pair<expression::Expression*, expression::Expression*>($2, $4));
                                    }
            ;

block:      '{'         {
                            syntax_analysis::IncreaseScope();
                            syntax_analysis::DefineStashedFormalArguments();
                        }
            stmts '}'   {
                            syntax_analysis::DecreaseScope();
                            DLOG("block -> { stmts }");
                        }
            ;

funcid:     ID              {
                                auto symbol = syntax_analysis::Lookup($1);
                                if (symbol == nullptr) {
                                    symbol = syntax_analysis::DefineNewSymbol(expression::USER_FUNC, $1, nullptr, yylineno);
                                }
                                else if (!symbol->is_active()) {
                                    syntax_analysis::SignalError("Cannot access " + symbol->get_id() + ", previously defined in line: " + std::to_string(symbol->get_line()), yylineno);
                                }
                                else {
                                    if (expression::IsVariable(symbol)) {
                                        syntax_analysis::SignalError(std::string($1) + " variable, previously defined in line: " + std::to_string(symbol->get_line()) + ", cannot be redefined as a function", yylineno);
                                    }
                                    else if (expression::IsLibraryFunction(symbol)) {
                                        syntax_analysis::SignalError(std::string($1) + " library function cannot be shadowed by a user function", yylineno);
                                    }
                                    else if (syntax_analysis::IsAtCurrentScope(symbol)) {
                                        syntax_analysis::SignalError("Name collision with function " + std::string($1) + ", previously defined in line: " + std::to_string(symbol->get_line()), yylineno);
                                    }
                                    else{
                                        symbol = syntax_analysis::DefineNewSymbol(expression::USER_FUNC, $1, nullptr, yylineno);  // shadow user function
                                    }
                                }
                                $$ = symbol;
                            }
            |               {
                                auto anonymous_function = syntax_analysis::DefineNewAnonymousFunc(yylineno);
                                $$ = anonymous_function;
                            }
            ;

funcprefix: FUNCTION funcid {
                                auto func_def_stmt = new intermediate_code::FuncDefStmt($2);

                                intermediate_code::func_def_stmts.push(func_def_stmt);

                                auto jump_quad = intermediate_code::Emit(intermediate_code::JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                intermediate_code::Emit(intermediate_code::FUNCSTART_t, $2, nullptr, nullptr, yylineno);

                                func_def_stmt->set_func_start_jump_quad(jump_quad);

                                $$ = $2;
                                
                                syntax_analysis::StoreFuncLocalOffset();
                                syntax_analysis::EnterScopeSpace();
                                syntax_analysis::ResetFormalArgOffset();
                            }

funcargs:   '(' idlist ')'  {
                                syntax_analysis::EnterScopeSpace();
                                syntax_analysis::RestoreFuncLocalOffset();
                                syntax_analysis::HideLowerScopes();
                                intermediate_code::stmt_stack.push_back(intermediate_code::FUNC_t);
                            }

funcbody:   block           {
                                $$ = syntax_analysis::CurrScopeOffset();
                                syntax_analysis::ExitScopeSpace();
                            }

funcdef:    funcprefix
            funcargs
            funcbody       {
                                syntax_analysis::ExitScopeSpace();
 
                                auto top_func_def = intermediate_code::func_def_stmts.top();
                                auto function = $1;
                                function->set_total_local($3);
                                auto func_end_quad = intermediate_code::Emit(intermediate_code::FUNCEND_t, function, nullptr, nullptr, yylineno);

                                top_func_def->PatchFuncStartJumpQuad(func_end_quad->label + 1);
                                top_func_def->PatchReturnJumpQuads(func_end_quad->label);

                                intermediate_code::func_def_stmts.pop();

                                syntax_analysis::EnableLowerScopes();

                                intermediate_code::stmt_stack.pop_back();

                                syntax_analysis::RestoreFuncLocalOffset();

                                $<bypass->funcdef>$ = top_func_def;

                                DLOG("funcdef -> function id (idlist) block"); 
                            }
            ;

const:        INTNUM    {
                            $$ = new expression::IntConstant($1);
                            DLOG("const -> INTNUM");
                        }
            | DOUBLENUM { 
                            $$ = new expression::DoubleConstant($1); 
                            DLOG("const -> DOUBLENUM"); 
                        }
            | STRING    { 
                            $$ = new expression::StringConstant(std::string($1));
                            DLOG("const -> STRING"); 
                        }
            | NIL       { 
                            $$ = new expression::NilConstant(nullptr);
                            DLOG("const -> NIL"); 
                        }
            | TRUE      { 
                            $$ = new expression::BoolConstant(true);
                            DLOG("const -> TRUE"); 
                        }
            | FALSE     {   
                            $$ = new expression::BoolConstant(false);
                            DLOG("const -> FALSE");
                        }
            ;

multid:     ',' ID  {
                        auto top_func_def_stmt = intermediate_code::func_def_stmts.top();
                        auto func = top_func_def_stmt->get_sym();
                        auto offset = syntax_analysis::CurrScopeOffset();
                        
                        syntax_analysis::IncreaseCurrOffset();

                        auto new_formal_arg = new expression::Symbol(expression::VAR, $2, yylineno, syntax_analysis::CurrScope() + 1, expression::FORMAL_ARG, offset, nullptr);

                        if (func->HasFormalArg(new_formal_arg)) {
                            syntax_analysis::SignalError("formal argument " + std::string($2) + " already declared", yylineno);
                        }
                        else {
                            func->AddFormalArg(new_formal_arg);
                            syntax_analysis::StashFormalArgument(new_formal_arg);       
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
                        auto top_func_def_stmt = intermediate_code::func_def_stmts.top();
                        auto func = top_func_def_stmt->get_sym();
                        auto offset = syntax_analysis::CurrScopeOffset();
                        syntax_analysis::IncreaseCurrOffset();

                        auto new_formal_arg = new expression::Symbol(expression::VAR, $1, yylineno, syntax_analysis::CurrScope() + 1, expression::FORMAL_ARG, offset, nullptr);
                        if (func->HasFormalArg(new_formal_arg)) {
                            syntax_analysis::SignalError("formal argument " + std::string($1) + " already declared", yylineno);
                        }
                        else {
                            func->AddFormalArg(new_formal_arg);   
                            syntax_analysis::StashFormalArgument(new_formal_arg);  
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
                                                if ($3->get_type() == expression::BOOL) {
                                                    $3 = intermediate_code::ConcludeShortCircuit(BOOL_EXPR_CAST($3), yylineno);
                                                }

                                                auto if_stmt = new intermediate_code::IfStmt();
                                                intermediate_code::if_stmts.push(if_stmt);

                                                auto branch_quad = intermediate_code::Emit(intermediate_code::IF_EQ_t, $3, new expression::BoolConstant(true), nullptr, yylineno);
                                                auto jump_quad = intermediate_code::Emit(intermediate_code::JUMP_t, nullptr, nullptr, nullptr, yylineno); 

                                                intermediate_code::PatchBranchQuad(branch_quad, branch_quad->label + 2);

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
                                auto top_if_stmt = intermediate_code::if_stmts.top();

                                auto else_jump_quad = intermediate_code::Emit(intermediate_code::JUMP_t, nullptr, nullptr, nullptr, yylineno);

                                top_if_stmt->PushElseJumpQuad(else_jump_quad);
                                top_if_stmt->PatchIfJumpQuad(intermediate_code::NextQuadLabel());
                            }
            stmt            {
                                auto top_if_stmt = intermediate_code::if_stmts.top();

                                top_if_stmt->PatchElseJumpQuad(intermediate_code::NextQuadLabel());
                                top_if_stmt->PopElseJumpQuad();

                                intermediate_code::if_stmts.pop();

                                //ResetTemp();
                                DLOG("elsestmt -> else stmt"); 
                            }
            |               {
                                auto top_if_stmt = intermediate_code::if_stmts.top();

                                top_if_stmt->PatchIfJumpQuad(intermediate_code::NextQuadLabel());

                                intermediate_code::if_stmts.pop();
                                DLOG("elsestmt -> EMPTY");
                            }
            ;

whilestmt:  WHILE               { 
                                    auto first_quad_label = intermediate_code::NextQuadLabel();

                                    auto while_stmt = new intermediate_code::WhileStmt(first_quad_label);

                                    intermediate_code::while_stmts.push(while_stmt);
                                    intermediate_code::loop_stmts.push(while_stmt);
                                }
            '(' expr ')'        {
                                    if ($4->get_type() == expression::BOOL) {
                                        $4 = intermediate_code::ConcludeShortCircuit(BOOL_EXPR_CAST($4), yylineno);
                                    }

                                    auto top_while_stmt = intermediate_code::while_stmts.top();

                                    auto branch_quad = intermediate_code::Emit(intermediate_code::IF_EQ_t, $4, new expression::BoolConstant(true), nullptr, yylineno);
                                    auto exit_quad = intermediate_code::Emit(intermediate_code::JUMP_t, nullptr, nullptr, nullptr, yylineno);

                                    top_while_stmt->PushLoopQuad(branch_quad);
                                    top_while_stmt->PushLoopQuad(exit_quad);

                                    intermediate_code::stmt_stack.push_back(intermediate_code::LOOP_t);
                                }
            stmt                { 
                                    auto top_while_stmt = intermediate_code::while_stmts.top();

                                    auto loop_quad = intermediate_code::Emit(intermediate_code::JUMP_t, nullptr, nullptr, nullptr, yylineno);

                                    top_while_stmt->PushLoopQuad(loop_quad);
                                    top_while_stmt->PatchLoopQuads();
                                    top_while_stmt->PatchBreakJumpQuads(intermediate_code::NextQuadLabel());
                                    top_while_stmt->PatchContinueJumpQuads();

                                    intermediate_code::while_stmts.pop();
                                    intermediate_code::loop_stmts.pop();

                                    intermediate_code::stmt_stack.pop_back();

                                    //ResetTemp();
                                    DLOG ("whilestmt -> WHILE (expr) stmt"); 
                                }
            ;

forstmt:    FOR                                     {
                                                        auto first_quad_label = intermediate_code::NextQuadLabel();

                                                        auto for_stmt = new intermediate_code::ForStmt(first_quad_label);

                                                        intermediate_code::for_stmts.push(for_stmt);
                                                        intermediate_code::loop_stmts.push(for_stmt);
                                                    }                   
            '(' elist ';'                           {
                                                        auto top_for_stmt = intermediate_code::for_stmts.top();

                                                        auto logical_expr_first_quad_label = intermediate_code::NextQuadLabel();

                                                        top_for_stmt->set_logical_expr_first_quad_label(logical_expr_first_quad_label);
                                                    }
            expr ';'                                {
                                                        if ($7->get_type() == expression::BOOL) {
                                                            $7 = intermediate_code::ConcludeShortCircuit(BOOL_EXPR_CAST($7), yylineno);
                                                        }

                                                        auto top_for_stmt = intermediate_code::for_stmts.top();

                                                        auto branch_quad = intermediate_code::Emit(intermediate_code::IF_EQ_t, $7, new expression::BoolConstant(true), nullptr, yylineno);

                                                        top_for_stmt->PushLoopQuad(branch_quad);

                                                        auto exit_quad = intermediate_code::Emit(intermediate_code::JUMP_t, nullptr, nullptr, nullptr, yylineno);

                                                        top_for_stmt->PushLoopQuad(exit_quad);

                                                        auto exprs_first_quad_label = intermediate_code::NextQuadLabel();

                                                        top_for_stmt->set_exprs_first_quad_label(exprs_first_quad_label);
                                                    }
            elist ')'                               {
                                                        auto top_for_stmt = intermediate_code::for_stmts.top();
                                                        auto loop_quad = intermediate_code::Emit(intermediate_code::JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                                        top_for_stmt->PushLoopQuad(loop_quad);
                                                        intermediate_code::stmt_stack.push_back(intermediate_code::LOOP_t);
                                                    }
            stmt                                    {
                                                        auto top_for_stmt = intermediate_code::for_stmts.top();
                                                        auto expr_jump_quad = intermediate_code::Emit(intermediate_code::JUMP_t, nullptr, nullptr, nullptr, yylineno); 

                                                        top_for_stmt->PushLoopQuad(expr_jump_quad);
                                                        top_for_stmt->PatchLoopQuads();
                                                        top_for_stmt->PatchBreakJumpQuads(intermediate_code::NextQuadLabel());
                                                        top_for_stmt->PatchContinueJumpQuads();

                                                        intermediate_code::for_stmts.pop();
                                                        intermediate_code::loop_stmts.pop(); 

                                                        intermediate_code::stmt_stack.pop_back();

                                                        //ResetTemp();
                                                        DLOG("forstmt -> FOR ( elist ; expr ; elist ) stmt"); 
                                                    }
            ;

returnstmt: RETURN      {
                            if (!intermediate_code::InFuncDef()) {
                                syntax_analysis::SignalError("Invalid return, used outside a function block", yylineno);
                            } else {
                                auto top_func_def = intermediate_code::func_def_stmts.top();
                                intermediate_code::Emit(intermediate_code::RET_t, nullptr, nullptr, nullptr, yylineno);
                                auto return_jump_quad = intermediate_code::Emit(intermediate_code::JUMP_t, nullptr, nullptr, nullptr, yylineno);

                                top_func_def->PushReturnJumpQuad(return_jump_quad);
                            }
                        } 
            ';'         {
                            DLOG("returnstmt -> RETURN;"); 
                        }
            | RETURN    
            expr ';'    {
                            if ($2->get_type() == expression::BOOL) {
                                $2 = intermediate_code::ConcludeShortCircuit(BOOL_EXPR_CAST($2), yylineno);
                            }

                            if (!intermediate_code::InFuncDef()) 
                                syntax_analysis::SignalError("Invalid return, used outside a function block", yylineno);
                            else {
                                auto top_func_def = intermediate_code::func_def_stmts.top();
                                intermediate_code::Emit(intermediate_code::RET_t, $2, nullptr, nullptr, yylineno);
                                auto return_jump_quad = intermediate_code::Emit(intermediate_code::JUMP_t, nullptr, nullptr, nullptr, yylineno);

                                top_func_def->PushReturnJumpQuad(return_jump_quad);
                                DLOG("returnstmt -> RETURN expr;");
                            }    
                        }
            ;

%%

int yyerror(std::string yaccProvidedMessage) {
    syntax_analysis::SignalError(yaccProvidedMessage, yylineno);
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
    syntax_analysis::InitLibraryFunctions();
    yyparse();
    #if defined LOGQUADS
        if (syntax_analysis::NoErrorSignaled()) {
            intermediate_code::LogQuads(std::cout);
        }
    #endif 
    #if defined LOGSYMTABLE
        if (syntax_analysis::NoErrorSignaled()) 
            syntax_analysis::LogSymTable(std::cout);
    #endif
    #if defined LOGQUADSTXT
        if (syntax_analysis::NoErrorSignaled()) {
            const char *path="../quads.txt";
            std::ofstream quad_file(path);
            intermediate_code::LogQuads(quad_file);
            quad_file.close();
        }
    #endif  

    return 0;
}
#endif