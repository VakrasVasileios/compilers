%{
    #include <stdio.h>
    #include <string>
    #include <iostream>
    #include "include/parser_manager/parser_manager.h"
    #include "include/parser_log/debuglog.h"
    
    int yyerror(std::string yaccProvidedMessage);
    int yylex(void);

    extern int yylineno;
    extern char* yytext;
    extern FILE* yyin;

    #define     LOGERROR(message)  std::cout << message << ", in line: " << yylineno << std::endl
    //#define     LOGERROR(message)  std::cout << "\033[1;31m" << message << ", in line: " << yylineno << "\033[0m" << std::endl  
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

program:      stmts                 { DLOG("program -> stmts"); }
            ;

stmts:        stmt stmts            { DLOG("stmts -> stmt stmts"); }
            |                       { DLOG("stmts -> EMPTY"); }
            ;

stmt:         expr ';'              { !IsMethodCall(); DLOG("stmt -> expr;");}
            | ifstmt                { DLOG("stmt -> ifstmt");}
            | whilestmt             { DLOG("stmt -> whilestmt");}
            | forstmt               { DLOG("stmt -> forstmt");}
            | returnstmt            { DLOG("stmt -> returnstmt");}
            | BREAK ';'             { 
                                        if(GetLoopDepth() == 0)
                                            LOGERROR("Error, invalid keyword BREAK outside of loop");
                                        DLOG("stmt -> break;");
                                    }
            | CONTINUE ';'          {
                                        if(GetLoopDepth() == 0)
                                            LOGERROR("Error, invalid keyword CONTINUE outside of loop");
                                        DLOG("stmt -> continue;");
                                    }
            | block                 { DLOG("stmt -> block");}
            | funcdef               { DLOG("stmt -> funcdef");}
            | ';'                   { DLOG("stmt -> ;");}
            ;

expr:         assignexpr            { DLOG("expr -> assignexpr"); }
            | expr '+' expr         { DLOG("expr -> expr + expr"); }
            | expr '-' expr         { DLOG("expr -> expr - expr"); }
            | expr '*' expr         { DLOG("expr -> expr * expr"); }
            | expr '/' expr         { DLOG("expr -> expr / expr"); }
            | expr '%' expr         { DLOG("expr -> expr % expr"); }
            | expr '>' expr         { DLOG("expr -> expr > expr"); }
            | expr GEQL expr        { DLOG("expr -> expr >= expr"); }
            | expr '<' expr         { DLOG("expr -> expr + expr"); }
            | expr LEQL expr        { DLOG("expr -> expr <= expr"); }
            | expr EQUAL expr       { DLOG("expr -> expr == expr"); }
            | expr NOTEQUAL expr    { DLOG("expr -> expr != expr"); }
            | expr AND expr         { DLOG("expr -> expr and expr"); }
            | expr OR expr          { DLOG("expr -> expr or expr"); }
            | term                  { DLOG("expr -> term"); }
            ;
term:         '(' expr ')'          { DLOG("term -> (expr)"); }
            | '-' expr %prec UMINUS { DLOG("term -> -expr"); }
            | NOT expr              { DLOG("term -> not expr"); }
            | PLUSPLUS lvalue       { DLOG("term -> ++lvalue"); }
            | lvalue PLUSPLUS       { DLOG("term -> lvalue++"); }
            | MINUSMINUS lvalue     { DLOG("term -> --lvaule"); }
            | lvalue MINUSMINUS     { DLOG("term -> lvalue--"); }
            | primary               { DLOG("term -> primary"); }
            ;

assignexpr:   lvalue '=' expr       { DLOG("assignexpr -> lvalue = expr"); }
            ;

primary:      lvalue                { DLOG("primary -> lvalue"); }
            | call                  { DLOG("primary -> call"); }
            | objectdef             { DLOG("primary -> objectdef"); }
            | '(' funcdef ')'       { DLOG("primary -> (funcdef)"); }
            | const                 { DLOG("primary -> const"); }
            ;

lvalue:       ID                    {
                                        $$=$1;
                                        auto item = Lookup($1);
                                        if (ScopeIsGlobal()) {
                                            if(item == nullptr)
                                                InsertGlobalVariable($1, yylineno);
                                            else if (IsLibraryFunction(item) || IsUserFunction(item)) {
                                                LOGERROR("Error," + std::string($1) + " is already in use as a function");
                                            }
                                        }
                                        else {
                                            if (item == nullptr) {
                                                InsertLocalVariable($1, yylineno);
                                            }
                                            else if (IsLibraryFunction(item) || IsUserFunction(item)) {
                                                LOGERROR("Error," + std::string($1) + " is already in use as a function");
                                            }
                                        }
                                        DLOG("lvalue -> id");
                                    }
            | LOCAL ID              {
                                        $$=$2;
                                        auto item = Lookup($2);
                                        if (item == nullptr) {
                                            InsertLocalVariable($2, yylineno);
                                        }
                                        else if (IsUserFunction(item) || IsLibraryFunction(item)) {
                                            LOGERROR("Error," + std::string($2) + " is already in use as a function");
                                        }
                                        DLOG("lvalue -> local id");
                                    }
            | COLONCOLON ID         {
                                        $$=$2;
                                        SymbolTableEntry* entry = Lookup($2);
                                        if (!IsLibraryFunction(entry) && !IsGlobalVariable(entry) && !IsUserFunction(entry))
                                            LOGERROR("No global variable with id: " + std::string($2));
                                        DLOG("lvalue -> ::id");
                                        }
            | member                { DLOG("lvalue -> member"); }
            ;

member:       lvalue '.' ID         { $$=$3; DLOG("member -> lvalue.id"); }
            | lvalue '[' expr ']'   { DLOG("member -> lvalue[expr]"); }   
            | call '.' ID           { $$=$3; DLOG("member -> call.id"); }
            | call '[' expr ']'     { DLOG("member -> call[expr]"); }
            ;
            
call:         call '(' elist ')'    { DLOG("call -> call(elist)"); }
            | lvalue callsuffix     {
                                        if(!IsMethodCall()) {
                                            SymbolTableEntry* entry = Lookup($1);
                                            if(!IsLibraryFunction(entry) && !IsUserFunction(entry))
                                                LOGERROR("No function with name: " + std::string($1));
                                        }
                                        DLOG("call -> lvalue callsuffix");
                                    }
            | '(' funcdef ')' '(' elist ')'  { DLOG("call -> (funcdef)(elist)"); }
            ;
            
callsuffix:   normcall              { DLOG("callsuffix -> normcall"); }
            | methodcall            { DLOG("callsuffix -> methodcall"); }
            ;

normcall:     '(' elist ')'           { DLOG("normcall -> (elist)"); }
            ;

methodcall:   DOTDOT ID '(' elist ')' { SetMethodCall(true); DLOG("methodcall -> ..id(elist)"); }
            ;

multelist:    ',' expr multelist    { DLOG("multelist -> ,expr multelist"); }
            |                       { DLOG("multelist -> EMPTY"); }
            ;

elist:        expr multelist        { DLOG("elist -> expr multelist"); }
            |                       { DLOG("elist -> EMPTY"); }
            ;

objectdef:    '[' elist ']'         { DLOG("objectdef -> [elist]"); }
            | '[' indexed ']'       { DLOG("objectdef -> [indexed]"); }
            ;

multindexed:  ',' indexedelem multindexed   { DLOG("multindexed -> , indexedelem multidexed"); }
            |                               { DLOG("elsestmt -> EMPTY"); }
            ;

indexed:      indexedelem multindexed    { DLOG("indexed -> indexedelem multidexed"); }
            ;

indexedelem:  '{' expr ':' expr '}'   { DLOG("indexedelem -> { expr : expr }"); }
            ;

block:        '{' {IncreaseScope();PushStashedFormalArguments();} stmts '}'  { DecreaseScope(); DLOG("block -> { stmts }"); }
            ;

funcdef:      FUNCTION {InsertUserFunction(yylineno);} '(' idlist ')' {HideLowerScopes();SetValidReturn(true);}  block {EnableLowerScopes();SetValidReturn(false); DLOG("funcdef -> function (idlist) block "); }
            | FUNCTION ID {InsertUserFunction($2, yylineno);} '(' idlist ')' {HideLowerScopes();SetValidReturn(true);} block {EnableLowerScopes();SetValidReturn(false); DLOG("funcdef -> function id (idlist) block"); }
            ;

const:        INTNUM                { DLOG("const -> INTNUM"); }
            | DOUBLENUM             { DLOG("const -> DOUBLENUM"); }
            | STRING                { DLOG("const -> STRING"); }
            | NIL                   { DLOG("const -> NIL"); }
            | TRUE                  { DLOG("const -> TRUE"); }
            | FALSE                 { DLOG("const -> FALSE"); }
            ;

multid:       ',' ID {StashFormalArgument($2, yylineno);} multid { DLOG("multid -> , id multid");}
            | {DLOG("multid -> EMPTY");}
            ;

idlist:       ID {StashFormalArgument($1, yylineno);} multid { DLOG("idlist -> id multid"); }
            | {DLOG("idlist -> EMPTY");}
            ;

ifstmt:       IF '(' expr ')' stmt elsestmt { DLOG("ifstmt -> if (expr) stmt elsestmt"); }
            ;

elsestmt:     ELSE stmt { DLOG("elsestmt -> else stmt"); }
            | {DLOG("elsestmt -> EMPTY");}
            ;

whilestmt:    WHILE { IncreaseLoopDepth();} '(' expr ')' stmt { DecreaseLoopDepth(); DLOG ("whilestmt -> WHILE (expr) stmt"); }
            ;

forstmt:      FOR { IncreaseLoopDepth();} '(' elist ';' expr ';' elist ')' stmt { DecreaseLoopDepth(); DLOG("forstmt -> FOR ( elist ; expr ; elist ) stmt"); }
            ;

returnstmt:   RETURN {if (!IsValidReturn()) LOGERROR("Invalid return, used outside a function block");} ';'  { DLOG("returnstmt -> RETURN;"); }
            | RETURN {if (!IsValidReturn()) LOGERROR("Invalid return, used outside a function block");} expr ';' { DLOG("returnstmt -> RETURN expr;"); }
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

    LogSymbolTable(std::cout);

    return 0;
}
#endif