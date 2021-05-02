%{
    #include <stdio.h>
    #include <string>
    #include <iostream>
    #include "include/parser_manager/ParserManager.h"
    #include "include/parser_log/DebugLog.h"
    
    int yyerror(std::string yaccProvidedMessage);
    int yylex(void);

    extern int yylineno;
    extern char* yytext;
    extern FILE* yyin;

    #define     log_error(message)  std::cout << message << ", in line: " << yylineno << std::endl
    //#define     log_error(message)  std::cout << "\033[1;31m" << message << ", in line: " << yylineno << "\033[0m" << std::endl  
%}

%union {                                                    
    char*       stringValue;
    int         intValue;
    double      doubleValue;
}

%start program

%token EQUAL NOTEQUAL COLONCOLON DOTDOT GEQL LEQL MINUSMINUS PLUSPLUS
%token LOCAL FUNCTION IF ELSE FOR WHILE BREAK CONTINUE NIL TRUE FALSE RETURN

%token <stringValue>    STRING ID
%token <intValue>       INTNUM
%token <doubleValue>    DOUBLENUM

%type <stringValue> lvalue member

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

program:      stmts                 { dlog("program -> stmts"); }
            ;

stmts:        stmt stmts            { dlog("stmts -> stmt stmts"); }
            |                       { dlog("stmts -> EMPTY"); }
            ;

stmt:         expr ';'              { set_method_call(false); dlog("stmt -> expr;");}
            | ifstmt                { dlog("stmt -> ifstmt");}
            | whilestmt             { dlog("stmt -> whilestmt");}
            | forstmt               { dlog("stmt -> forstmt");}
            | returnstmt            { dlog("stmt -> returnstmt");}
            | BREAK ';'             { 
                                        if(get_loop_depth() == 0)
                                            log_error("Error, invalid keyword BREAK outside of loop");
                                        dlog("stmt -> break;");
                                    }
            | CONTINUE ';'          {
                                        if(get_loop_depth() == 0)
                                            log_error("Error, invalid keyword CONTINUE outside of loop");
                                        dlog("stmt -> continue;");
                                    }
            | block                 { dlog("stmt -> block");}
            | funcdef               { dlog("stmt -> funcdef");}
            | ';'                   { dlog("stmt -> ;");}
            ;

expr:         assignexpr            { dlog("expr -> assignexpr"); }
            | expr '+' expr         { dlog("expr -> expr + expr"); }
            | expr '-' expr         { dlog("expr -> expr - expr"); }
            | expr '*' expr         { dlog("expr -> expr * expr"); }
            | expr '/' expr         { dlog("expr -> expr / expr"); }
            | expr '%' expr         { dlog("expr -> expr % expr"); }
            | expr '>' expr         { dlog("expr -> expr > expr"); }
            | expr GEQL expr        { dlog("expr -> expr >= expr"); }
            | expr '<' expr         { dlog("expr -> expr + expr"); }
            | expr LEQL expr        { dlog("expr -> expr <= expr"); }
            | expr EQUAL expr       { dlog("expr -> expr == expr"); }
            | expr NOTEQUAL expr    { dlog("expr -> expr != expr"); }
            | expr AND expr         { dlog("expr -> expr and expr"); }
            | expr OR expr          { dlog("expr -> expr or expr"); }
            | term                  { dlog("expr -> term"); }
            ;
term:         '(' expr ')'          { dlog("term -> (expr)"); }
            | '-' expr %prec UMINUS { dlog("term -> -expr"); }
            | NOT expr              { dlog("term -> not expr"); }
            | PLUSPLUS lvalue       { dlog("term -> ++lvalue"); }
            | lvalue PLUSPLUS       { dlog("term -> lvalue++"); }
            | MINUSMINUS lvalue     { dlog("term -> --lvaule"); }
            | lvalue MINUSMINUS     { dlog("term -> lvalue--"); }
            | primary               { dlog("term -> primary"); }
            ;

assignexpr:   lvalue '=' expr  { dlog("assignexpr -> lvalue = expr"); }
            ;

primary:      lvalue                { dlog("primary -> lvalue"); }
            | call                  { dlog("primary -> call"); }
            | objectdef             { dlog("primary -> objectdef"); }
            | '(' funcdef ')'       { dlog("primary -> (funcdef)"); }
            | const                 { dlog("primary -> const"); }
            ;

lvalue:       ID                    {
                                        $$=$1;
                                        auto item = lookup($1);
                                        if (scope_is_global()) {
                                            if(item == nullptr)
                                                insert_variable_global($1, yylineno);
                                            else if (is_library_function(item) || is_user_function(item)) {
                                                log_error("Error," + std::string($1) + " is already in use as a function");
                                            }
                                        }
                                        else {
                                            if (item == nullptr) {
                                                insert_variable_local($1, yylineno);
                                            }
                                            else if (is_library_function(item) || is_user_function(item)) {
                                                log_error("Error," + std::string($1) + " is already in use as a function");
                                            }
                                        }
                                        dlog("lvalue -> id");
                                    }
            | LOCAL ID              {
                                        $$=$2;
                                        auto item = lookup($2);
                                        if (item == nullptr) {
                                            insert_variable_local($2, yylineno);
                                        }
                                        else if (is_user_function(item) || is_library_function(item)) {
                                            log_error("Error," + std::string($2) + " is already in use as a function");
                                        }
                                        dlog("lvalue -> local id");
                                    }
            | COLONCOLON ID         {
                                        $$=$2;
                                        SymbolTableEntry* entry = lookup($2);
                                        if (entry == nullptr)
                                            log_error("No global variable with id: " + std::string($2));
                                        dlog("lvalue -> ::id");
                                        }
            | member                { dlog("lvalue -> member"); }
            ;

member:       lvalue '.' ID         { $$=$3; dlog("member -> lvalue.id"); }
            | lvalue '[' expr ']'   { dlog("member -> lvalue[expr]"); }   
            | call '.' ID           { $$=$3; dlog("member -> call.id"); }
            | call '[' expr ']'     { dlog("member -> call[expr]"); }
            ;
            
call:         call '(' elist ')'    { dlog("call -> call(elist)"); }
            | lvalue callsuffix     {
                                        if(!is_method_call()) {
                                            SymbolTableEntry* entry = lookup($1);
                                            if(entry == nullptr)
                                                log_error("No function with name: " + std::string($1));
                                        }
                                        dlog("call -> lvalue callsuffix");
                                    }
            | '(' funcdef ')' '(' elist ')'  { dlog("call -> (funcdef)(elist)"); }
            ;
            
callsuffix:   normcall              { dlog("callsuffix -> normcall"); }
            | methodcall            { dlog("callsuffix -> methodcall"); }
            ;

normcall:     '(' elist ')'           { dlog("normcall -> (elist)"); }
            ;

methodcall:   DOTDOT ID '(' elist ')' { set_method_call(true); dlog("methodcall -> ..id(elist)"); }
            ;

multelist:    ',' expr multelist    { dlog("multelist -> ,expr multelist"); }
            |                       { dlog("multelist -> EMPTY"); }
            ;

elist:        expr multelist        { dlog("elist -> expr multelist"); }
            |                       { dlog("elist -> EMPTY"); }
            ;

objectdef:    '[' elist ']'         { dlog("objectdef -> [elist]"); }
            | '[' indexed ']'       { dlog("objectdef -> [indexed]"); }
            ;

multindexed:  ',' indexedelem multindexed   { dlog("multindexed -> , indexedelem multidexed"); }
            |                               { dlog("elsestmt -> EMPTY"); }
            ;

indexed:      indexedelem multindexed    { dlog("indexed -> indexedelem multidexed"); }
            ;

indexedelem:  '{' expr ':' expr '}'   { dlog("indexedelem -> { expr : expr }"); }
            ;

block:        '{' {increase_scope();push_stashed_formal_arguments();} stmts '}'  { decrease_scope(); dlog("block -> { stmts }"); }
            ;

funcdef:      FUNCTION {insert_user_function(yylineno);} '(' idlist ')' {hide_lower_scopes();set_valid_return(true);}  block {enable_lower_scopes();set_valid_return(false); dlog("funcdef -> function (idlist) block "); }
            | FUNCTION ID {
                                auto item = lookup($2);
                                if (item == nullptr)
                                    insert_user_function($2, yylineno);
                                else {
                                    if (is_local_variable(item) || is_formal_variable(item) || is_global_variable(item))
                                        log_error("Error, " + std::string($2) + " variable cannot be redefined as a function");
                                    else if (is_library_function(item))
                                        log_error("Error, " + std::string($2) + " library function cannot be shadowed by a user function");
                                    else if (item->getScope() == get_current_scope()) {
                                        std::string message = "Error, " + std::string($2) + " name collision with function defined in line ";
                                        message += (int)item->getLine();
                                        log_error(message);
                                    }
                                    else
                                        insert_user_function($2, yylineno);
                                }
                          }
              '(' idlist ')' {hide_lower_scopes();set_valid_return(true);} block {enable_lower_scopes();set_valid_return(false); dlog("funcdef -> function id (idlist) block"); }
            ;

const:        INTNUM                { dlog("const -> INTNUM"); }
            | DOUBLENUM             { dlog("const -> DOUBLENUM"); }
            | STRING                { dlog("const -> STRING"); }
            | NIL                   { dlog("const -> NIL"); }
            | TRUE                  { dlog("const -> TRUE"); }
            | FALSE                 { dlog("const -> FALSE"); }
            ;

multid:       ',' ID {stash_formal_argument($2, yylineno);} multid { dlog("multid -> , id multid");}
            | {dlog("multid -> EMPTY");}
            ;

idlist:       ID {stash_formal_argument($1, yylineno);} multid { dlog("idlist -> id multid"); }
            | {dlog("idlist -> EMPTY");}
            ;

ifstmt:       IF '(' expr ')' stmt elsestmt { dlog("ifstmt -> if (expr) stmt elsestmt"); }
            ;

elsestmt:     ELSE stmt { dlog("elsestmt -> else stmt"); }
            | {dlog("elsestmt -> EMPTY");}
            ;

whilestmt:    WHILE { increase_loop_depth();} '(' expr ')' stmt { decrease_loop_depth(); dlog ("whilestmt -> WHILE (expr) stmt"); }
            ;

forstmt:      FOR { increase_loop_depth();} '(' elist ';' expr ';' elist ')' stmt { decrease_loop_depth(); dlog("forstmt -> FOR ( elist ; expr ; elist ) stmt"); }
            ;

returnstmt:   RETURN {if (!is_valid_return()) log_error("Invalid return, used outside a function block");} ';'  { dlog("returnstmt -> RETURN;"); }
            | RETURN {if (!is_valid_return()) log_error("Invalid return, used outside a function block");} expr ';' { dlog("returnstmt -> RETURN expr;"); }
            ;

%%

int yyerror(std::string yaccProvidedMessage) {
    std::cout << yaccProvidedMessage << ": at line " << yylineno << ", before token: " << yytext << std::endl;
    std::cout << "INPUT NOT VALID" << std::endl;
    return 1;
}

#ifndef TESTING
int main(int argc, char** argv) {    
    init_library_functions();
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

    log_symbol_table(std::cout);

    return 0;
}
#endif