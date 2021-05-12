%{
    #include <stdio.h>
    #include <string>
    #include <iostream>
    #include "include/parser_manager/parser_manager.h"
    #include "include/parser_log/debuglog.h"
    #include <assert.h>
    
    int yyerror(std::string yaccProvidedMessage);
    int yylex(void);

    extern int yylineno;
    extern char* yytext;
    extern FILE* yyin;

    // #define     SIGNALERROR(message)   std::cout << "\033[31mError, in line: " << yylineno << ":\033[0m " << message << std::endl
    // #define     LOGWARNING(message) std::cout << "\033[33mWarning, in line: " << yylineno << ":\033[0m " << message << std::endl  

    #define     SIGNALERROR(message)  \
        do { \
            std::cout << "Error, in line: " << yylineno << ": " << message << std::endl; \
            SignalError(); \
        } while (0)

    #define     LOGWARNING(message) std::cout << "Warning, in line: " << yylineno << ": " << message << std::endl 
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

stmts:        stmt stmts            {
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
                                        }
                                        DLOG("stmt -> break;");
                                    }
            | CONTINUE ';'          {
                                        if(GetLoopDepth() == 0)
                                            SIGNALERROR("invalid keyword CONTINUE outside of loop");
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
                                        DLOG("expr -> assignexpr");
                                    }
            | expr '+' expr         {
                                        // auto entry1 = Lookup($1);
                                        // auto entry2 = Lookup($3);
                                        // if(entry1 == nullptr)
                                        //     LOGERROR("Cannot access " + entry1->get_id() + ", previously defined in line: " + std::to_string(entry1->get_line()));
                                        // else if (!entry1->is_active())
                                        //     LOGERROR("Cannot access " + entry1->get_id() + ", previously defined in line: " + std::to_string(entry1->get_line()));
                                        // else if(entry2 == nullptr)
                                        //     LOGERROR("Cannot access " + entry2->get_id() + ", previously defined in line: " + std::to_string(entry2->get_line()));
                                        // else if (!entry2->is_active())
                                        //     LOGERROR("Cannot access " + entry2->get_id() + ", previously defined in line: " + std::to_string(entry2->get_line()));
                                        // else if (!IsVariable(entry1) || (!IsVariable(entry2))
                                        //     LOGERROR("Use of addition with non variable type");
                                        //     //boolean + string check      
                                        // else{
                                        //     auto _t1 = NewTemp();
                                        //     $$ = _t1;
                                        //     Emit(ADD_t, _t1, entry1, entry2, yylineno);
                                        // } 
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
                                        DLOG("expr -> expr > expr");
                                    }
            | expr GEQL expr        {
                                        DLOG("expr -> expr >= expr");
                                    }
            | expr '<' expr         {
                                        DLOG("expr -> expr + expr");
                                    }
            | expr LEQL expr        {
                                        DLOG("expr -> expr <= expr");
                                    }
            | expr EQUAL expr       {
                                        DLOG("expr -> expr == expr");
                                    }
            | expr NOTEQUAL expr    {
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
                                        if ($1 == nullptr || $1 == NULL) {
                                            LOGWARNING("Attempting to assign a value to NIL");
                                            // $$ = lval;
                                            // Emit(ASSIGN_t, lval, nullptr, $2);
                                        }
                                        else {
                                            auto symbol = $1;
                                            if (symbol == nullptr) {
                                                LOGWARNING("Attempting to assign a value to NIL");
                                                // $$ = lval;
                                                // Emit(ASSIGN_t, lval, nullptr, $2);
                                            }
                                            else if (IsLibraryFunction(symbol) || IsUserFunction(symbol))
                                                SIGNALERROR("Functions are constant their value cannot be changed");
                                            else {
                                                $$ = symbol;
                                                Emit(ASSIGN_t, symbol, nullptr, $3, yylineno);
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
                                        if (ScopeIsGlobal()) {
                                            auto entry = LookupGlobal($1);
                                            if(entry == nullptr) {
                                                $$ = InsertGlobalVariable($1, yylineno);
                                            }
                                            else if (!entry->is_active()) {
                                                SIGNALERROR("Cannot access " + entry->get_id() + ", previously defined in line: " + std::to_string(entry->get_line()));
                                                $$ = entry;
                                            }
                                            else {
                                                $$ = entry;
                                            }
                                        }
                                        else {
                                            auto entry = Lookup($1);
                                            if (entry == nullptr) {
                                                $$ = InsertLocalVariable($1, yylineno);
                                            }
                                            else if (!entry->is_active()) {
                                                SIGNALERROR("Cannot access " + entry->get_id() + ", previously defined in line: " + std::to_string(entry->get_line()));
                                                $$ = entry;
                                            }
                                            else {
                                                $$ = entry;
                                            }
                                        }
                                        DLOG("lvalue -> id");
                                    }
            | LOCAL ID              {
                                        auto entry = Lookup($2);
                                        if (entry == nullptr) { 
                                            $$ = InsertLocalVariable($2, yylineno);
                                        }
                                        else if (!entry->is_active()) {
                                            SIGNALERROR("Cannot access " + entry->get_id() + ", previously defined in line: " + std::to_string(entry->get_line()));
                                            $$ = entry;
                                        }
                                        else if (IsGlobalVar(entry)) {
                                            $$ = InsertLocalVariable($2, yylineno);
                                        }
                                        else if (IsUserFunction(entry)){
                                            if(IsAtCurrentScope(entry)) {
                                                SIGNALERROR("Attempting to redefine a previously declared user function");
                                                $$ = entry;
                                            }
                                            else {
                                                $$ = InsertLocalVariable($2, yylineno);
                                            }    
                                        }
                                        else if (IsLibraryFunction(entry)) {
                                            SIGNALERROR("Attempting to redefine a library function");
                                            $$ = entry;
                                        }
                                        else {
                                            $$ = entry;
                                        }
                                        DLOG("lvalue -> local id");
                                    }
            | COLONCOLON ID         {
                                        auto entry = LookupGlobal($2);
                                        if (entry == nullptr || !entry->is_active()) {
                                            SIGNALERROR("No global variable with id: " + std::string($2));
                                            $$ = entry;
                                        }
                                        else {
                                            $$ = entry;
                                        }
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

                                                // if (called_function == nullptr) {
                                                //     LOGWARNING("Attempting use of function call with NIL value");
                                                //     called_function = new UserFunction($1, yylineno, GetCurrentScope()); 
                                                // }
                                                auto function_call = new FunctionCall(called_symbol, std::list<Expression*>());
                                                
                                                NewCallStackFrame(function_call);
                                                IncreaseFunctionDepth();

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

                                                IncreaseTemp();

                                                if (IsLibraryFunction(called_symbol) || IsUserFunction(called_symbol)) {
                                                    auto args_num = static_cast<Function*>(called_symbol)->get_formal_arguments().size();
                                                    auto call_args_num = function_call->get_params().size();

                                                    if (call_args_num < args_num)
                                                        SIGNALERROR("Too few arguments passed to function: " << called_symbol->get_id() << ", defined in line: " << std::to_string(called_symbol->get_line()));
                                                    else if (call_args_num > args_num)
                                                        LOGWARNING("Too many arguments passed to function: " << called_symbol->get_id() << ", defined in line: " << std::to_string(called_symbol->get_line()));
                                                }

                                                DecreaseFunctionDepth();

                                                if (GetFunctionDepth() == 0)
                                                    ResetTemp();

                                                DLOG("call -> lvalue callsuffix");
                                            }
            | '(' funcdef ')'               {
                                                IncreaseFunctionDepth();
                                                
                                                auto called_symbol = $2;
                                                auto function_call = new FunctionCall(called_symbol, std::list<Expression*>());

                                                NewCallStackFrame(function_call);

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

                                                IncreaseTemp();

                                                if (IsLibraryFunction(called_symbol) || IsUserFunction(called_symbol)) {
                                                    auto args_num = static_cast<Function*>(called_symbol)->get_formal_arguments().size();
                                                    auto call_args_num = function_call->get_params().size();

                                                    if (call_args_num < args_num)
                                                        SIGNALERROR("Too few arguments passed to function: " << called_symbol->get_id() << ", defined in line: " << std::to_string(called_symbol->get_line()));
                                                    else if (call_args_num > args_num)
                                                        LOGWARNING("Too many arguments passed to function: " << called_symbol->get_id() << ", defined in line: " << std::to_string(called_symbol->get_line()));
                                                }

                                                DecreaseFunctionDepth();

                                                if (GetFunctionDepth() == 0)
                                                    ResetTemp();

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
                                    if (GetFunctionDepth() != 0) {
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
                                if (GetFunctionDepth() != 0) {
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
                                $<sym>$ = function;
                                Emit(FUNCSTART_t, function, nullptr, nullptr, yylineno);
                                HideLowerScopes();
                                IncreaseReturnDepth();
                            }
            block           {
                                auto function = LookupPreviousFunction();
                                $<sym>$ = function;
                                Emit(FUNCEND_t, function, nullptr, nullptr, yylineno);
                                EnableLowerScopes();
                                DecreaseReturnDepth();
                                DLOG("funcdef -> function (idlist) block "); 
                            }
            | FUNCTION ID 
                '(' idlist ')'
                            {
                                auto entry = Lookup($2);
                                if (entry == nullptr) {
                                    auto function = InsertUserFunction($2, yylineno);
                                    $<sym>$ = function;
                                    Emit(FUNCSTART_t, function, nullptr, nullptr, yylineno);
                                }
                                else if (!entry->is_active()) {
                                    SIGNALERROR("Cannot access " + entry->get_id() + ", previously defined in line: " + std::to_string(entry->get_line()));
                                    $<sym>$ = entry;
                                }
                                else {
                                    if (IsVariable(entry)) {
                                        SIGNALERROR(std::string($2) + " variable, previously defined in line: " + std::to_string(entry->get_line()) + ", cannot be redefined as a function");
                                        $<sym>$ = entry;
                                    }
                                    else if (IsLibraryFunction(entry)) {
                                        SIGNALERROR(std::string($2) + " library function cannot be shadowed by a user function");
                                        $<sym>$ = entry;
                                    }
                                    else if (IsAtCurrentScope(entry)) {
                                        std::string message =  "Name collision with function " +  std::string($2) + ", previously defined in line: ";
                                        message += std::to_string(entry->get_line());
                                        SIGNALERROR(message);
                                        $<sym>$ = entry;
                                    }
                                    else{
                                        auto function = InsertUserFunction($2, yylineno); //Shadow user function. 
                                        $<sym>$ = function;
                                        Emit(FUNCSTART_t, function, nullptr, nullptr, yylineno);
                                    }
                                }
                                HideLowerScopes();
                                IncreaseReturnDepth();
                            }
            block           { 
                                auto function = LookupPreviousFunction();
                                $<sym>$ = function;
                                Emit(FUNCEND_t, function, nullptr, nullptr, yylineno);
                                EnableLowerScopes();
                                DecreaseReturnDepth();
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

ifstmt:     IF '(' expr ')' stmt elsestmt   {
                                                DLOG("ifstmt -> if (expr) stmt elsestmt"); 
                                            }
            ;

elsestmt:   ELSE stmt       {
                                DLOG("elsestmt -> else stmt"); 
                            }
            |               {
                                DLOG("elsestmt -> EMPTY");
                            }
            ;

whilestmt:  WHILE               { 
                                    IncreaseLoopDepth();
                                }
            '(' expr ')' stmt   { 
                                    DecreaseLoopDepth();
                                    DLOG ("whilestmt -> WHILE (expr) stmt"); 
                                }
            ;

forstmt:    FOR                                     {
                                                        IncreaseLoopDepth();
                                                    }
            '(' elist ';' expr ';' elist ')' stmt   {
                                                        DecreaseLoopDepth();
                                                        DLOG("forstmt -> FOR ( elist ; expr ; elist ) stmt"); 
                                                    }
            ;

returnstmt: RETURN      {
                            if (GetReturnDepth() == 0)
                                SIGNALERROR("Invalid return, used outside a function block");

                            Emit(RET_t, nullptr, nullptr, nullptr, yylineno);
                            //Emit(JUMP_t, nullptr, nullptr, nullptr, yylineno);
                        } 
            ';'         {
                            DLOG("returnstmt -> RETURN;"); 
                        }
            | RETURN    {
                            if (GetReturnDepth() == 0) 
                                SIGNALERROR("Invalid return, used outside a function block");
                        }
            expr ';'    {
                           // Emit(RET_t, $3, nullptr, nullptr, yylineno);
                            DLOG("returnstmt -> RETURN expr;");
                        }
            ;

%%

int yyerror(std::string yaccProvidedMessage) {
    std::cout << yaccProvidedMessage << ": at line " << yylineno << ", before token: " << yytext << std::endl;
    std::cout << "INPUT NOT VALID" << std::endl;
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

    if (NoErrorSignaled())
        LogQuads(std::cout);

    // if (NoErrorSignaled())
    //     LogSymbolTable(std::cout);

    return 0;
}
#endif