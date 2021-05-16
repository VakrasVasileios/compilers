%{
    #include <stdio.h>
    #include <string>
    #include <iostream>
    #include "include/parser_manager.h"
    #include "include/debuglog.h"
    #include <assert.h>
    
    int yyerror(std::string yaccProvidedMessage);
    int yylex(void);

    extern int yylineno;
    extern char* yytext;
    extern FILE* yyin;

    #if !defined TEST
        #define     SIGNALERROR(message)  \
            do { \
                std::cout << "\033[31mError, in line: " << yylineno << ":\033[0m " << message << std::endl; \
                SignalError(); \
            } while (0)

        #define     LOGWARNING(message) std::cout << "\033[33mWarning, in line: " << yylineno << ":\033[0m " << message << std::endl 
    #else 
        #define     SIGNALERROR(message)  \
            do { \
                std::cout << "Error, in line: " << yylineno << ": " << message << std::endl; \
                SignalError(); \
            } while (0)

        #define     LOGWARNING(message) std::cout << "Warning, in line: " << yylineno << ": " << message << std::endl 
    #endif
%}

%union {                                                    
    char*                   stringValue;
    int                     intValue;
    double                  doubleValue;
    class Expression*       expression;
    class Constant*         con;
    class FunctionCall*     funcCall;
    class Symbol*           sym;
}

%start program

%token EQUAL NOTEQUAL COLONCOLON DOTDOT GEQL LEQL MINUSMINUS PLUSPLUS
%token LOCAL FUNCTION IF ELSE FOR WHILE BREAK CONTINUE NIL TRUE FALSE RETURN

%token <stringValue>    STRING ID 
%token <intValue>       INTNUM
%token <doubleValue>    DOUBLENUM

%type <expression> primary term expr assignexpr
%type <con> const
%type <sym> lvalue funcdef
%type <stringValue> member
%type <funcCall> call

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
                                        if(GetLoopDepth() == 0) {
                                            SIGNALERROR("invalid keyword BREAK outside of loop");
                                        } else {
                                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                            auto jump_label = TopLoopStartLabel();
                                            PushLoopBreakJumpQuad(jump_label, jump_quad);
                                        }
                                        DLOG("stmt -> break;");
                                    }
            | CONTINUE ';'          {
                                        if(GetLoopDepth() == 0) {
                                            SIGNALERROR("invalid keyword CONTINUE outside of loop");
                                        } else {
                                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                            auto jump_label = TopLoopStartLabel();
                                            PushLoopContinueJumpQuad(jump_label, jump_quad);
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
                                        auto temp = NewTemp();
                                        Emit(ASSIGN_t, temp, $1, nullptr, yylineno);

                                        $$ = temp;

                                        DLOG("expr -> assignexpr");
                                    }
            | expr '+' expr         {
                                        DLOG("expr -> expr + expr");
                                    }
            | expr '-' expr         {
                                        DLOG("expr -> expr - expr");
                                    }
            | expr '*' expr         {
                                        DLOG("expr -> expr * expr");
                                    }
            | expr '/' expr         {
                                        DLOG("expr -> expr / expr");
                                    }
            | expr '%' expr         {
                                        DLOG("expr -> expr % expr");
                                    }
            | expr '>' expr         {
                                        auto greater_quad = Emit(IF_GREATER_t, $1, $3, nullptr, yylineno);
                                        PatchBranchQuad(greater_quad, greater_quad->label + 2);

                                        auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                        PatchJumpQuad(jump_quad, jump_quad->label + 3);

                                        auto temp = NewTemp();

                                        Emit(ASSIGN_t, temp, new BoolConstant(true), nullptr, yylineno);

                                        jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                        PatchJumpQuad(jump_quad, jump_quad->label + 2);

                                        Emit(ASSIGN_t, temp, new BoolConstant(false), nullptr, yylineno);

                                        $$ = temp;

                                        DLOG("expr -> expr > expr");
                                    }
            | expr GEQL expr        {
                                        auto greater_equal_quad = Emit(IF_GREATEREQ_t, $1, $3, nullptr, yylineno);
                                        PatchBranchQuad(greater_equal_quad, greater_equal_quad->label + 2);

                                        auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                        PatchJumpQuad(jump_quad, jump_quad->label + 3);

                                        auto temp = NewTemp();

                                        Emit(ASSIGN_t, temp, new BoolConstant(true), nullptr, yylineno);

                                        jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                        PatchJumpQuad(jump_quad, jump_quad->label + 2);

                                        Emit(ASSIGN_t, temp, new BoolConstant(false), nullptr, yylineno);

                                        $$ = temp;

                                        DLOG("expr -> expr >= expr");
                                    }
            | expr '<' expr         {
                                        auto less_quad = Emit(IF_LESS_t, $1, $3, nullptr, yylineno);
                                        PatchBranchQuad(less_quad, less_quad->label + 2);

                                        auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                        PatchJumpQuad(jump_quad, jump_quad->label + 3);

                                        auto temp = NewTemp();

                                        Emit(ASSIGN_t, temp, new BoolConstant(true), nullptr, yylineno);

                                        jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                        PatchJumpQuad(jump_quad, jump_quad->label + 2);

                                        Emit(ASSIGN_t, temp, new BoolConstant(false), nullptr, yylineno);

                                        $$ = temp;

                                        DLOG("expr -> expr + expr");
                                    }
            | expr LEQL expr        {
                                        auto less_equal_quad = Emit(IF_LESSEQ_t, $1, $3, nullptr, yylineno);
                                        PatchBranchQuad(less_equal_quad, less_equal_quad->label + 2);

                                        auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                        PatchJumpQuad(jump_quad, jump_quad->label + 3);

                                        auto temp = NewTemp();

                                        Emit(ASSIGN_t, temp, new BoolConstant(true), nullptr, yylineno);

                                        jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                        PatchJumpQuad(jump_quad, jump_quad->label + 2);

                                        Emit(ASSIGN_t, temp, new BoolConstant(false), nullptr, yylineno);

                                        $$ = temp;

                                        DLOG("expr -> expr <= expr");
                                    }
            | expr EQUAL expr       {
                                        auto equal_quad = Emit(IF_EQ_t, $1, $3, nullptr, yylineno);
                                        PatchBranchQuad(equal_quad, equal_quad->label + 2);

                                        auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                        PatchJumpQuad(jump_quad, jump_quad->label + 3);

                                        auto temp = NewTemp();

                                        Emit(ASSIGN_t, temp, new BoolConstant(true), nullptr, yylineno);

                                        jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                        PatchJumpQuad(jump_quad, jump_quad->label + 2);

                                        Emit(ASSIGN_t, temp, new BoolConstant(false), nullptr, yylineno);

                                        $$ = temp;

                                        DLOG("expr -> expr == expr");
                                    }
            | expr NOTEQUAL expr    {
                                        auto not_equal_quad = Emit(IF_NOTEQ_t, $1, $3, nullptr, yylineno);
                                        PatchBranchQuad(not_equal_quad, not_equal_quad->label + 2);

                                        auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                        PatchJumpQuad(jump_quad, jump_quad->label + 3);

                                        auto temp = NewTemp();

                                        Emit(ASSIGN_t, temp, new BoolConstant(true), nullptr, yylineno);

                                        jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                        PatchJumpQuad(jump_quad, jump_quad->label + 2);

                                        Emit(ASSIGN_t, temp, new BoolConstant(false), nullptr, yylineno);

                                        $$ = temp;

                                        DLOG("expr -> expr != expr");
                                    }
            | expr AND expr         {
                                        DLOG("expr -> expr and expr");
                                    }
            | expr OR expr          {
                                        DLOG("expr -> expr or expr");
                                    }
            | term                  {
                                        DLOG("expr -> term");
                                    }
            ;

term:         '(' expr ')'          {
                                        DLOG("term -> (expr)");
                                    }
            | '-' expr %prec UMINUS {
                                        DLOG("term -> -expr");
                                    }
            | NOT expr              {
                                        DLOG("term -> not expr");
                                    }
            | PLUSPLUS lvalue       {
                                        auto symbol = $2;
                                        if(symbol == nullptr)
                                            SIGNALERROR("Cannot access " + symbol->get_id() + ", previously defined in line: " + std::to_string(symbol->get_line()));
                                        else if (!symbol->is_active())
                                            SIGNALERROR("Cannot access " + symbol->get_id() + ", previously defined in line: " + std::to_string(symbol->get_line()));    
                                        else if (!IsVariable(symbol))
                                            SIGNALERROR("Use of increment operator with non variable type");    

                                        DLOG("term -> ++lvalue"); 
                                    }
            | lvalue PLUSPLUS       {
                                        auto symbol = $1;
                                        if(symbol == nullptr)
                                            SIGNALERROR("Cannot access " + symbol->get_id() + ", previously defined in line: " + std::to_string(symbol->get_line()));
                                        else if (!symbol->is_active())
                                            SIGNALERROR("Cannot access " + symbol->get_id() + ", previously defined in line: " + std::to_string(symbol->get_line()));    
                                        else if (!IsVariable(symbol))
                                            SIGNALERROR("Use of increment operator with non variable type");    

                                        auto temp = NewTemp(); 

                                        Emit(ASSIGN_t, temp, symbol, nullptr, yylineno);    
                                        Emit(ADD_t, symbol, symbol, new IntConstant(1), yylineno);

                                        $$ = temp;

                                        DLOG("term -> lvalue++"); }
            | MINUSMINUS lvalue     { 
                                        auto symbol = $2;
                                        if(symbol == nullptr)
                                            SIGNALERROR("Cannot access " + symbol->get_id() + ", previously defined in line: " + std::to_string(symbol->get_line()));
                                        else if (!symbol->is_active())
                                            SIGNALERROR("Cannot access " + symbol->get_id() + ", previously defined in line: " + std::to_string(symbol->get_line()));    
                                        else if (!IsVariable(symbol))
                                            SIGNALERROR("Use of decrement operator with non variable type");    
                                        DLOG("term -> --lvaule");
                                    }
            | lvalue MINUSMINUS     { 
                                        auto symbol = $1;
                                        if(symbol == nullptr)
                                            SIGNALERROR("Cannot access " + symbol->get_id() + ", previously defined in line: " + std::to_string(symbol->get_line()));
                                        else if (!symbol->is_active())
                                            SIGNALERROR("Cannot access " + symbol->get_id() + ", previously defined in line: " + std::to_string(symbol->get_line()));    
                                        else if (!IsVariable(symbol))
                                            SIGNALERROR("Use of decrement operator with non variable type");
                                        DLOG("term -> lvalue--");
                                    }
            | primary               {   
                                        DLOG("term -> primary"); 
                                    }
            ;

assignexpr:   lvalue '=' expr       {
                                        
                                        auto symbol = $1;
                                        if (symbol != nullptr) {
                                            if (IsLibraryFunction(symbol) || IsUserFunction(symbol)) {
                                                SIGNALERROR("Functions are constant their value cannot be changed");
                                            }
                                            else {
                                                auto assign_quad = Emit(ASSIGN_t, symbol, nullptr, $3, yylineno);
                                                $$ = assign_quad->result;
                                            }
                                        }
                                            
                                        DLOG("assignexpr -> lvalue = expr");
                                    }
            ;

primary:      lvalue                {
                                        $$ = $1;
                                        DLOG("primary -> lvalue");
                                    }
            | call                  {
                                        $$ = $1;
                                        DLOG("primary -> call");
                                    }
            | objectdef             {
                                        DLOG("primary -> objectdef");
                                    }
            | '(' funcdef ')'       {
                                        $$ = $2;
                                        DLOG("primary -> (funcdef)");
                                    }
            | const                 {
                                        $$ = $1;
                                        DLOG("primary -> const");
                                    }
            ;

lvalue:       ID                    {
                                        Symbol* symbol;
                                        if (ScopeIsGlobal()) {
                                            symbol = LookupGlobal($1);
                                            if(symbol == nullptr) {
                                                symbol = InsertGlobalVariable($1, yylineno);
                                            }
                                            else if (!symbol->is_active()) {
                                                SIGNALERROR("Cannot access " + symbol->get_id() + ", previously defined in line: " + std::to_string(symbol->get_line()));
                                            }
                                            $$ = symbol;
                                        }
                                        else {
                                            symbol = Lookup($1);
                                            if (symbol == nullptr) {
                                                symbol = InsertLocalVariable($1, yylineno);
                                            }
                                            else if (!symbol->is_active()) {
                                                SIGNALERROR("Cannot access " + symbol->get_id() + ", previously defined in line: " + std::to_string(symbol->get_line()));
                                            }
                                            $$ = symbol;
                                        }
                                        DLOG("lvalue -> id");
                                    }
            | LOCAL ID              {
                                        auto symbol = Lookup($2);
                                        if (symbol == nullptr) { 
                                            symbol = InsertLocalVariable($2, yylineno);
                                        }
                                        else if (!symbol->is_active()) {
                                            SIGNALERROR("Cannot access " + symbol->get_id() + ", previously defined in line: " + std::to_string(symbol->get_line()));
                                        }
                                        else if (IsGlobalVar(symbol)) {
                                            symbol = InsertLocalVariable($2, yylineno);
                                        }
                                        else if (IsUserFunction(symbol)){
                                            if(IsAtCurrentScope(symbol)) {
                                                SIGNALERROR("Attempting to redefine a previously declared user function");
                                            }
                                            else {
                                                symbol = InsertLocalVariable($2, yylineno);
                                            }    
                                        }
                                        else if (IsLibraryFunction(symbol)) {
                                            SIGNALERROR("Attempting to redefine a library function");
                                        }
                                        $$ = symbol;
                                        DLOG("lvalue -> local id");
                                    }
            | COLONCOLON ID         {
                                        auto symbol = LookupGlobal($2);
                                        if (symbol == nullptr || !symbol->is_active()) 
                                            SIGNALERROR("No global variable with id: " + std::string($2));

                                        $$ = symbol;
                                        DLOG("lvalue -> ::id");
                                    }
            | member                {
                                        DLOG("lvalue -> member");
                                    }
            ;

member:     lvalue '.' ID           {
                                        $$=$3;
                                        DLOG("member -> lvalue.id");
                                    }
            | lvalue '[' expr ']'   {
                                        DLOG("member -> lvalue[expr]");
                                    }
            | call '.' ID           {
                                        $$=$3;
                                        DLOG("member -> call.id");
                                    }
            | call '[' expr ']'     {
                                        DLOG("member -> call[expr]");
                                    }
            ;

call:       call  '(' elist ')'             {
                                                DLOG("call -> call(elist)");
                                            }
            | lvalue                        {
                                                
                                                auto called_symbol = $1;
                                                auto function_call = new FunctionCall(called_symbol, std::list<Expression*>());
                                                
                                                PushCallStackFrame(function_call);

                                                $<funcCall>$ = function_call;
                                            }
            callsuffix                      {
                                                auto function_call = PopCallStackFrame();
                                                auto called_symbol = function_call->get_called_symbol();

                                                $<funcCall>$ = function_call;

                                                auto temp_value = NewTemp();
                        
                                                Emit(CALL_t, called_symbol, nullptr, nullptr, yylineno);    
                                                Emit(GETRETVAL_t, temp_value, nullptr, nullptr, yylineno);

                                                function_call->set_ret_val(temp_value->get_id());

                                                if (IsLibraryFunction(called_symbol) || IsUserFunction(called_symbol)) {
                                                    auto args_num = static_cast<Function*>(called_symbol)->get_formal_arguments().size();
                                                    auto call_args_num = function_call->get_params().size();

                                                    if (call_args_num < args_num)
                                                        SIGNALERROR("Too few arguments passed to function: " << called_symbol->get_id() << ", defined in line: " << std::to_string(called_symbol->get_line()));
                                                    else if (call_args_num > args_num)
                                                        LOGWARNING("Too many arguments passed to function: " << called_symbol->get_id() << ", defined in line: " << std::to_string(called_symbol->get_line()));
                                                }

                                                DLOG("call -> lvalue callsuffix");
                                            }
            | '(' funcdef ')'               {                                                
                                                auto called_symbol = $2;
                                                auto function_call = new FunctionCall(called_symbol, std::list<Expression*>());

                                                PushCallStackFrame(function_call);

                                                $<funcCall>$ = function_call;
                                            }
            '(' elist ')'                   {
                                                auto function_call = PopCallStackFrame();
                                                auto called_symbol = function_call->get_called_symbol();

                                                $<funcCall>$ = function_call;

                                                auto temp_value = NewTemp();

                                                Emit(CALL_t, called_symbol, nullptr, nullptr, yylineno);
                                                Emit(GETRETVAL_t, temp_value, nullptr, nullptr, yylineno);

                                                function_call->set_ret_val(temp_value->get_id());

                                                if (IsLibraryFunction(called_symbol) || IsUserFunction(called_symbol)) {
                                                    auto args_num = static_cast<Function*>(called_symbol)->get_formal_arguments().size();
                                                    auto call_args_num = function_call->get_params().size();

                                                    if (call_args_num < args_num)
                                                        SIGNALERROR("Too few arguments passed to function: " << called_symbol->get_id() << ", defined in line: " << std::to_string(called_symbol->get_line()));
                                                    else if (call_args_num > args_num)
                                                        LOGWARNING("Too many arguments passed to function: " << called_symbol->get_id() << ", defined in line: " << std::to_string(called_symbol->get_line()));
                                                }

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
                                    if (GetCallDepth() != 0) {
                                        PushCallParam($2);
                                        Emit(PARAM_t, $2, nullptr, nullptr, yylineno);
                                    }
                                    DLOG("multelist -> ,expr multelist");
                                }
            |                   {
                                    DLOG("multelist -> EMPTY");
                                }
            ;

elist:      expr multelist  {
                                if (GetCallDepth() != 0) {
                                    PushCallParam($1);
                                    Emit(PARAM_t, $1, nullptr, nullptr, yylineno);
                                }
                                             
                                DLOG("elist -> expr multelist");
                            }
            |               {
                                DLOG("elist -> EMPTY");
                            }
            ;

objectdef:  '[' elist ']'       {
                                    DLOG("objectdef -> [elist]");
                                }
            | '[' indexed ']'   { 
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
                                        DLOG("indexedelem -> { expr : expr }"); 
                                    }
            ;

block:      '{'         {
                            IncreaseScope();
                            PushStashedFormalArguments();
                        }
            stmts '}'   {
                            DecreaseScope();
                            DLOG("block -> { stmts }");
                        }
            ;

funcdef:    FUNCTION 
                '(' idlist ')'  
                            {
                                auto function = InsertUserFunction(yylineno);
                                auto function_def = new FunctionDef(function);
                                PushFuncDef(function_def);
                                
                                auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                Emit(FUNCSTART_t, function, nullptr, nullptr, yylineno);

                                MapJumpQuad(function_def, jump_quad);

                                HideLowerScopes();

                                $<sym>$ = function;
                            }
            block           {
                                auto func_def = PopFuncDef();
                                auto function = func_def->get_sym();
                                
                                auto func_end_quad = Emit(FUNCEND_t, function, nullptr, nullptr, yylineno);

                                PatchJumpQuad(func_def, func_end_quad->label + 1);
                                PatchJumpQuadList(func_def, func_end_quad->label);

                                EnableLowerScopes();

                                $<sym>$ = function;
                                DLOG("funcdef -> function (idlist) block "); 
                            }
            | FUNCTION ID 
                '(' idlist ')'
                            {
                                auto symbol = Lookup($2);
                                if (symbol == nullptr) {
                                    auto function = InsertUserFunction($2, yylineno);
                                    symbol = function;
                                }
                                else if (!symbol->is_active()) {
                                    SIGNALERROR("Cannot access " + symbol->get_id() + ", previously defined in line: " + std::to_string(symbol->get_line()));
                                }
                                else {
                                    if (IsVariable(symbol)) {
                                        SIGNALERROR(std::string($2) + " variable, previously defined in line: " + std::to_string(symbol->get_line()) + ", cannot be redefined as a function");
                                    }
                                    else if (IsLibraryFunction(symbol)) {
                                        SIGNALERROR(std::string($2) + " library function cannot be shadowed by a user function");
                                    }
                                    else if (IsAtCurrentScope(symbol)) {
                                        std::string message =  "Name collision with function " +  std::string($2) + ", previously defined in line: ";
                                        message += std::to_string(symbol->get_line());
                                        SIGNALERROR(message);
                                    }
                                    else{
                                        auto function = InsertUserFunction($2, yylineno); //Shadow user function. 
                                        symbol = function;
                                    }
                                }
                                auto function_def = new FunctionDef(symbol);
                                PushFuncDef(function_def);

                                auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                Emit(FUNCSTART_t, symbol, nullptr, nullptr, yylineno);

                                MapJumpQuad(function_def, jump_quad);

                                HideLowerScopes();

                                $<sym>$ = symbol;
                            }
            block           { 
                                auto func_def =  PopFuncDef();
                                Symbol* function;
                                if (func_def != nullptr) {
                                    function = func_def->get_sym();
                                    auto func_end_quad = Emit(FUNCEND_t, function, nullptr, nullptr, yylineno);
                                    PatchJumpQuad(func_def, func_end_quad->label + 1);
                                    PatchJumpQuadList(func_def, func_end_quad->label);
                                }
                                
                                EnableLowerScopes();

                                $<sym>$ = function;

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
                            $$ = new StringConstant($1);
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
                        StashFormalArgument($2, yylineno);
                    } 
            multid  {
                        DLOG("multid -> , id multid");
                    }
            |       {
                        DLOG("multid -> EMPTY");
                    }
            ;

idlist:     ID      {
                        StashFormalArgument($1, yylineno);
                    } 
            multid  { 
                        DLOG("idlist -> id multid"); 
                    }
            |       {
                        DLOG("idlist -> EMPTY");
                    }
            ;

ifstmt:     IF '(' expr ')'                 {
                                                IncreaseIfStmtDepth();

                                                auto if_stmt_depth = GetIfStmtDepth();

                                                auto branch_quad = Emit(IF_EQ_t, $3, new BoolConstant(true), nullptr, yylineno);
                                                PatchBranchQuad(branch_quad, branch_quad->label + 2);

                                                auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno); 
                                                MapIfStmtJumpQuad(if_stmt_depth, jump_quad);
                                            }
            stmt                            {
                                                auto if_stmt_depth = GetIfStmtDepth();

                                                PatchIfStmtJumpQuad(if_stmt_depth);

                                                DecreaseIfStmtDepth(); //?0

                                                ResetTemp();
                                            }
            elsestmt                        {
                                                DLOG("ifstmt -> if (expr) stmt elsestmt"); 
                                            }
            ;

elsestmt:   ELSE stmt       {
                                ResetTemp();
                                DLOG("elsestmt -> else stmt"); 
                            }
            |               {
                                DLOG("elsestmt -> EMPTY");
                            }
            ;

whilestmt:  WHILE               { 
                                    PushLoopStartLabel(GetBackQuadLabel() + 1);
                                }
            '(' expr ')'        {
                                    auto top_loop_start_label = TopLoopStartLabel();

                                    auto branch_quad = Emit(IF_EQ_t, $4, new BoolConstant(true), nullptr, yylineno);
                                    auto exit_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);

                                    PushLoopBranchQuad(top_loop_start_label, branch_quad);
                                    PushLoopBranchQuad(top_loop_start_label, exit_quad);
                                }
            stmt                { 
                                    unsigned int top_loop_start_label = TopLoopStartLabel();

                                    auto loop_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                    
                                    PushLoopBranchQuad(top_loop_start_label, loop_quad);

                                    PatchWhileLoopBranchQuads(top_loop_start_label);
                                    PatchLoopBreakJumpQuads(top_loop_start_label, loop_quad->label + 1);
                                    PatchWhileLoopContinueJumpQuads(top_loop_start_label);

                                    PopLoopStartLabel();

                                    ResetTemp();

                                    DLOG ("whilestmt -> WHILE (expr) stmt"); 
                                }
            ;

forstmt:    FOR                                     {
                                                        PushLoopStartLabel(GetBackQuadLabel() + 1);
                                                    }
            '(' elist ';'                           {
                                                        auto top_loop_start_label = TopLoopStartLabel();
                                                        auto logical_expr_start_label = GetBackQuadLabel() + 1;

                                                        MapLogicalExpressionStartLabel(top_loop_start_label, logical_expr_start_label);
                                                    }
            expr ';'                                {
                                                        auto top_loop_start_label = TopLoopStartLabel();

                                                        auto branch_quad = Emit(IF_EQ_t, $7, new BoolConstant(true), nullptr, yylineno);
                                                        PushLoopBranchQuad(top_loop_start_label, branch_quad);

                                                        auto exit_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                                        PushLoopBranchQuad(top_loop_start_label, exit_quad);

                                                        auto exprs_start_label = GetBackQuadLabel() + 1;
                                                        MapExpressionsStartLabel(top_loop_start_label, exprs_start_label);
                                                    }
            elist ')'                               {
                                                        auto top_loop_start_label = TopLoopStartLabel();

                                                        auto loop_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                                                        PushLoopBranchQuad(top_loop_start_label, loop_quad);
                                                    }
            stmt                                    {
                                                        auto top_loop_start_label = PopLoopStartLabel();

                                                        auto expr_jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);  
                                                        PushLoopBranchQuad(top_loop_start_label, expr_jump_quad);

                                                        PatchForLoopBranchQuads(top_loop_start_label);
                                                        PatchLoopBreakJumpQuads(top_loop_start_label, GetBackQuadLabel() + 1);
                                                        PatchForLoopContinueJumpQuads(top_loop_start_label);

                                                        ResetTemp();

                                                        DLOG("forstmt -> FOR ( elist ; expr ; elist ) stmt"); 
                                                    }
            ;

returnstmt: RETURN      {
                            if (GetFuncDefDepth() == 0) {
                                SIGNALERROR("Invalid return, used outside a function block");
                            }

                            Emit(RET_t, nullptr, nullptr, nullptr, yylineno);
                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);

                            PushJumpQuad(TopFuncDef(), jump_quad);
                        } 
            ';'         {
                            DLOG("returnstmt -> RETURN;"); 
                        }
            | RETURN    {
                            if (GetFuncDefDepth() == 0) 
                                SIGNALERROR("Invalid return, used outside a function block");
                        }
            expr ';'    {
                            Emit(RET_t, $3, nullptr, nullptr, yylineno);
                            auto jump_quad = Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);

                            PushJumpQuad(TopFuncDef(), jump_quad);

                            DLOG("returnstmt -> RETURN expr;");
                        }
            ;

%%

int yyerror(std::string yaccProvidedMessage) {
    std::cout << yaccProvidedMessage << ": at line " << yylineno << ", before token: " << yytext << std::endl;
    std::cout << "INPUT NOT VALID" << std::endl;
    SignalError();
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
        if (NoErrorSignaled()) 
            LogQuads(std::cout);
    #endif         
    #if defined LOGSYMTABLE
        if (NoErrorSignaled()) 
            LogSymbolTable(std::cout); 
    #endif

    return 0;
}
#endif