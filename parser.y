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


    #define     LOGERROR(message)   std::cout << "\033[31mError, in line: " << yylineno << ":\033[0m " << message << std::endl
    #define     LOGWARNING(message) std::cout << "\033[33mWarning, in line: " << yylineno << ":\033[0m " << message << std::endl    
    //#define       LOGERROR(message)  std::cout << "Error, " << "in line: " << yylineno << ". " << message  << std::endl
    //#define     LOGERROR(message)  std::cout << "Error, " << "\033[31m" << message << ", in line: " << yylineno << "\033[0m" << std::endl  
%}

%union {                                                    
    char*                   stringValue;
    int                     intValue;
    double                  doubleValue;
    class Expression*       expression;
    class Constant*         con;
}

%start program

%token EQUAL NOTEQUAL COLONCOLON DOTDOT GEQL LEQL MINUSMINUS PLUSPLUS
%token LOCAL FUNCTION IF ELSE FOR WHILE BREAK CONTINUE NIL TRUE FALSE RETURN

%token <stringValue>    STRING ID 
%token <intValue>       INTNUM
%token <doubleValue>    DOUBLENUM

%type <expression> primary term expr assignexpr
%type <con> const
%type <stringValue> member lvalue

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
                                        SetMethodCall(false);
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
                                        if(GetLoopDepth() == 0)
                                            LOGERROR("invalid keyword BREAK outside of loop");
                                        DLOG("stmt -> break;");
                                    }
            | CONTINUE ';'          {
                                        if(GetLoopDepth() == 0)
                                            LOGERROR("invalid keyword CONTINUE outside of loop");
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
                                        auto entry = Lookup($2);
                                        if(entry == nullptr)
                                            LOGERROR("Attempting to increase a NIL constant");
                                        else if (!entry->is_active())
                                            LOGERROR("Cannot access " + entry->get_id() + ", previously defined in line: " + std::to_string(entry->get_line()));    
                                        else if (!IsVariable(entry))
                                            LOGERROR("Use of increment operator with non variable type");    
                                        DLOG("term -> ++lvalue"); 
                                    }
            | lvalue PLUSPLUS       {
                                        auto entry = Lookup($1);
                                        if(entry == nullptr)
                                            LOGERROR("Attempting to increase a NIL constant");
                                        else if (!entry->is_active())
                                            LOGERROR("Cannot access " + entry->get_id() + ", previously defined in line: " + std::to_string(entry->get_line()));    
                                        else if (!IsVariable(entry))
                                            LOGERROR("Use of increment operator with non variable type");    
                                        DLOG("term -> lvalue++"); }
            | MINUSMINUS lvalue     { 
                                        auto entry = Lookup($2);
                                        if(entry == nullptr)
                                            LOGERROR("Attempting to decrease a NIL constant");
                                        else if (!entry->is_active())
                                            LOGERROR("Cannot access " + entry->get_id() + ", previously defined in line: " + std::to_string(entry->get_line()));    
                                        else if (!IsVariable(entry))
                                            LOGERROR("Use of decrement operator with non variable type");    
                                        DLOG("term -> --lvaule"); 
                                    }
            | lvalue MINUSMINUS     { 
                                        auto entry = Lookup($1);
                                        if(entry == nullptr)
                                            LOGERROR("Attempting to decrease a NIL constant");
                                        else if (!entry->is_active())
                                            LOGERROR("Cannot access " + entry->get_id() + ", previously defined in line: " + std::to_string(entry->get_line()));    
                                        else if (!IsVariable(entry))
                                            LOGERROR("Use of decrement operator with non variable type");
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
                                            auto lval = Lookup($1);
                                            if (lval == nullptr) {
                                                LOGWARNING("Attempting to assign a value to NIL");
                                                // $$ = lval;
                                                // Emit(ASSIGN_t, lval, nullptr, $2);
                                            }
                                            else if (IsLibraryFunction(lval) || IsUserFunction(lval))
                                                LOGERROR("Functions are constant their value cannot be changed");
                                            else {
                                                $$ = lval;
                                                Emit(ASSIGN_t, lval, nullptr, $3);
                                            }
                                        }
                                        DLOG("assignexpr -> lvalue = expr");
                                    }
            ;

primary:      lvalue                {
                                        DLOG("primary -> lvalue");
                                    }
            | call                  {
                                        DLOG("primary -> call");
                                    }
            | objectdef             {
                                        DLOG("primary -> objectdef");
                                    }
            | '(' funcdef ')'       {
                                        DLOG("primary -> (funcdef)");
                                    }
            | const                 {
                                        DLOG("primary -> const");
                                    }
            ;

lvalue:       ID                    {
                                        $$=$1;
                                        if (ScopeIsGlobal()) {
                                            auto entry = LookupGlobal($1);
                                            if(entry == nullptr) {
                                                InsertGlobalVariable($1, yylineno);
                                            }
                                            else if (!entry->is_active())
                                                LOGERROR("Cannot access " + entry->get_id() + ", previously defined in line: " + std::to_string(entry->get_line()));
                                        }
                                        else {
                                            auto entry = Lookup($1);
                                            if (entry == nullptr) {
                                                InsertLocalVariable($1, yylineno);
                                            }
                                            else if (!entry->is_active())
                                                LOGERROR("Cannot access " + entry->get_id() + ", previously defined in line: " + std::to_string(entry->get_line()));
                                        }
                                        DLOG("lvalue -> id");
                                    }
            | LOCAL ID              {
                                        $$=$2;
                                        auto entry = Lookup($2);
                                        if (entry == nullptr) { 
                                            InsertLocalVariable($2, yylineno);
                                        }
                                        else if (!entry->is_active())
                                            LOGERROR("Cannot access " + entry->get_id() + ", previously defined in line: " + std::to_string(entry->get_line()));
                                        else if (IsGlobalVar(entry)) {
                                            InsertLocalVariable($2, yylineno);
                                        }
                                        else if (IsUserFunction(entry)){
                                            if(IsAtCurrentScope(entry)) {
                                                LOGERROR("Attempting to redefine a previously declared user function");
                                            }
                                            else {
                                                InsertLocalVariable($2, yylineno);
                                            }    
                                        }
                                        else if (IsLibraryFunction(entry)) {
                                            LOGERROR("Attempting to redefine a library function");
                                        }
                                        DLOG("lvalue -> local id");
                                    }
            | COLONCOLON ID         {
                                        $$ = $2;
                                        auto entry = LookupGlobal($2);
                                        if (entry == nullptr || !entry->is_active()) {
                                            LOGERROR("No global variable with id: " + std::string($2));
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

call:       call '(' elist ')'              {
                                                DLOG("call -> call(elist)");
                                            }
            | lvalue callsuffix             {
                                                if(!IsMethodCall()) {
                                                    auto entry = LookupFunc($1);
                                                    if (entry == nullptr || !entry->is_active())
                                                        LOGERROR("Attempting use of function call with NIL value");
                                                }
                                                DLOG("call -> lvalue callsuffix");
                                            }
            | '(' funcdef ')' '(' elist ')' {
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
                                        SetMethodCall(true);
                                        DLOG("methodcall -> ..id(elist)");
                                    }
            ;

multelist:  ',' expr multelist  {
                                    DLOG("multelist -> ,expr multelist");
                                }
            |                   {
                                    DLOG("multelist -> EMPTY");
                                }
            ;

elist:      expr multelist  {
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

funcdef:    FUNCTION        {
                                InsertUserFunction(yylineno);
                            } 
            '(' idlist ')'  {
                                HideLowerScopes();
                                IncreaseReturnDepth();
                            }  
            block           {
                                EnableLowerScopes();
                                DecreaseReturnDepth();
                                DLOG("funcdef -> function (idlist) block "); 
                            }
            | FUNCTION ID   {
                                auto entry = Lookup($2);
                                if (entry == nullptr)
                                    InsertUserFunction($2, yylineno);
                                 else if (!entry->is_active())
                                        LOGERROR("Cannot access " + entry->get_id() + ", previously defined in line: " + std::to_string(entry->get_line()));   
                                else {
                                    if (IsVariable(entry))
                                        LOGERROR(std::string($2) + " variable, previously defined in line: " + std::to_string(entry->get_line()) + ", cannot be redefined as a function");
                                    else if (IsLibraryFunction(entry))
                                        LOGERROR(std::string($2) + " library function cannot be shadowed by a user function");
                                    else if (IsAtCurrentScope(entry)) {
                                        std::string message =  "Name collision with function " +  std::string($2) + ", previously defined in line: ";
                                        message += std::to_string(entry->get_line());
                                        LOGERROR(message);
                                    }
                                    else
                                        InsertUserFunction($2, yylineno); //Shadow user function. 
                                }
                            }
            '(' idlist ')'  { 
                                HideLowerScopes();
                                IncreaseReturnDepth();
                            }
            block           { 
                                EnableLowerScopes();
                                DecreaseReturnDepth();
                                DLOG("funcdef -> function id (idlist) block"); 
                            }
            ;

const:        INTNUM    {
                            $$ = new Constant($1);
                            DLOG("const -> INTNUM");
                        }
            | DOUBLENUM { 
                            $$ = new Constant($1); 
                            DLOG("const -> DOUBLENUM"); 
                        }
            | STRING    { 
                            $$ = new Constant($1);
                            DLOG("const -> STRING"); 
                        }
            | NIL       { 
                            $$ = new Constant(nullptr);
                            DLOG("const -> NIL"); 
                        }
            | TRUE      { 
                            $$ = new Constant(true);
                            DLOG("const -> TRUE"); 
                        }
            | FALSE     {   
                            $$ = new Constant(false);
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
                                LOGERROR("Invalid return, used outside a function block");
                        } 
            ';'         {
                            DLOG("returnstmt -> RETURN;"); 
                        }
            | RETURN    {
                            if (GetReturnDepth() == 0) 
                                LOGERROR("Invalid return, used outside a function block");
                        }
            expr ';'    {
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
    //SE KATOYRAW
    InitLibraryFunctions();

    yyparse();

    LogQuads(std::cout);

    //LogSymbolTable(std::cout);

    return 0;
}
#endif