%{
    #include <stdio.h>
    #include <string>
    #include <iostream>
    #include "include/Scope.h"
    #include "include/DebugLog.h"
    
    int yyerror(std::string yaccProvidedMessage);
    int yylex(void);

    extern int yylineno;
    extern char* yytext;
    extern FILE* yyin;
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

program:      stmt                  { dlog("program -> stmt");}
            | stmt program          { dlog("program -> stmt program");}
            ;

stmt:         expr ';'              { dlog("stmt -> expr;");}
            | ifstmt                { dlog("stmt -> ifstmt");}
            | whilestmt             { dlog("stmt -> whilestmt");}
            | forstmt               { dlog("stmt -> forstmt");}
            | returnstmt            { dlog("stmt -> returnstmt");}
            | BREAK ';'             { if(GetLoopDepth() == 0) std::cout << "Error, invalid keyword BREAK outside of loop, in line: "<< yylineno << std::endl; dlog("stmt -> break;");}
            | CONTINUE ';'          { if(GetLoopDepth() == 0) std::cout << "Error, invalid keyword CONTINUE outside of loop in line: "<< yylineno << std::endl; dlog("stmt -> continue;");}
            | block                 { dlog("stmt -> block");}
            | funcdef               { dlog("stmt -> funcdef");}
            | ';'                   { dlog("stmt -> ;");}
            ;

expr:         assignexpr            { dlog("expr -> assignexpr");}
            | expr '+' expr         { dlog("expr -> expr + expr"); }
            | expr '-' expr         { dlog("expr -> expr - expr"); }
            | expr '*' expr         { dlog("expr -> expr * expr"); }
            | expr '/' expr         { dlog("expr -> expr / expr"); }
            | expr '%' expr         { dlog("expr -> expr % expr"); }
            | expr '>' expr         { dlog("expr -> expr > expr"); }
            | expr GEQL expr        { dlog("expr -> expr >= expr");}
            | expr '<' expr         { dlog("expr -> expr + expr"); }
            | expr LEQL expr        { dlog("expr -> expr <= expr");}
            | expr EQUAL expr       { dlog("expr -> expr == expr");}
            | expr NOTEQUAL expr    { dlog("expr -> expr != expr");}
            | expr AND expr         { dlog("expr -> expr and expr"); }
            | expr OR expr          { dlog("expr -> expr or expr"); }
            | term                  { dlog("expr -> term");}
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

assignexpr:   lvalue {AddStashedLvalues();} '=' expr  { dlog("assignexpr -> lvalue = expr"); }
            ;

primary:      lvalue                { dlog("primary -> lvalue"); }
            | call                  { dlog("primary -> call"); }
            | objectdef             { dlog("primary -> objectdef"); }
            | '(' funcdef ')'       { dlog("primary -> (funcdef)"); }
            | const                 { dlog("primary -> const"); }
            ;

lvalue:       ID                    { $$=$1; if(GetScope() == 0)StashLvalue($1, yylineno, GLOBAL_VAR); else StashLvalue($1, yylineno, LOCAL_VAR); dlog("lvalue -> id"); }
            | LOCAL ID              { $$=$2; StashLvalue($2, yylineno, LOCAL_VAR); dlog("lvalue -> local id"); }
            | COLONCOLON ID         { $$=$2; if (Lookup($2, GLOBAL_VAR) == nullptr) std::cout << "No global variable with id: "<< $2 << ", i line: " << yylineno << std::endl; dlog("lvalue -> ::id"); }
            | member                { dlog("lvalue -> member"); }
            ;

member:       lvalue '.' ID           { $$=$3; dlog("member -> lvalue.id"); }
            | lvalue '[' expr ']'     { dlog("member -> lvalue[expr]"); }
            | call '.' ID             { $$=$3; dlog("member -> call.id"); }
            | call '[' expr ']'       { dlog("member -> call[expr]"); }
            ;
            
call:         call '(' elist ')'        { dlog("call -> call(elist)"); }
            | lvalue callsuffix         { dlog("call -> lvalue callsuffix"); }
            | '(' funcdef ')' '(' elist ')'  { dlog("call -> (funcdef)(elist)"); }
            ;
            
callsuffix:   normcall              { dlog("callsuffix -> normcall"); }
            | methodcall            { dlog("callsuffix -> methodcall"); }
            ;

normcall:     '(' elist ')'           { dlog("normcall -> (elist)"); }
            ;

methodcall:   DOTDOT ID '(' elist ')' { if(Lookup($2, LIB_FUNC) == nullptr && Lookup($2, USER_FUNC) == nullptr) std::cout << "No function with that name in line: " << yylineno << std::endl; dlog("methodcall -> ..id(elist)"); }
            ;

multelist:    ',' expr multelist    { dlog("multelist -> ,expr multelist"); }
            |
            ;
elist:        expr multelist        { dlog("elist -> expr multelist"); }
            |
            ;

objectdef:    '[' elist ']'         { dlog("objectdef -> [elist]"); }
            | '[' indexed ']'       { dlog("objectdef -> [indexed]"); }
            ;

multindexed:  ',' indexedelem multindexed { dlog("multindexed -> , indexedelem multidexed"); }
            |
            ;

indexed:      indexedelem multindexed    { dlog("indexed -> indexedelem multidexed"); }
            ;

indexedelem:  '{' expr ':' expr '}'   { dlog("indexedelem -> { expr : expr }"); }
            ;

block:        '{' {IncreaseScope();AddFormalArgs();} program '}'  { DecreaseScope(); dlog("block -> { program }"); }
            | '{' {IncreaseScope();AddFormalArgs();} '}'          { DecreaseScope(); dlog("block -> {}");}
            ;

funcdef:      FUNCTION {AddAnonymousFunction(yylineno);} '(' idlist ')' {HideLowerScopes();}  block {EnableLowerScopes(); dlog("funcdef -> function (idlist) block "); }
            | FUNCTION ID {AddFunction($2, yylineno);} '(' idlist ')' {HideLowerScopes();} block {EnableLowerScopes(); dlog("funcdef -> function id (idlist) block"); }
            ;

const:        INTNUM                { dlog("const -> INTNUM"); }
            | DOUBLENUM             { dlog("const -> DOUBLENUM");}
            | STRING                { dlog("const -> STRING"); }
            | NIL                   { dlog("const -> NIL"); }
            | TRUE                  { dlog ("const -> TRUE"); }
            | FALSE                 { dlog ("const -> FALSE"); }
            ;

multid:       ',' ID {GetArgList().push_back({std::string($2), yylineno});} multid { dlog("multid -> , id multid");}
            |
            ;

idlist:       ID {GetArgList().push_back({std::string($1), yylineno});} multid { dlog("idlist -> id multid"); }
            |
            ;

ifstmt:       IF '(' expr ')' stmt elsestmt { dlog("ifstmt -> if (expr) stmt elsestmt"); }
            ;

elsestmt:     ELSE stmt { dlog("elsestmt -> else stmt"); }
            |
            ;

whilestmt:    WHILE { IncreaseLoopDepth(); } '(' expr ')' stmt { DecreaseLoopDepth(); dlog ("whilestmt -> WHILE (expr) stmt"); }
            ;

forstmt:      FOR { IncreaseLoopDepth(); } '(' elist ';' expr ';' elist ')' stmt { DecreaseLoopDepth(); dlog("forstmt -> FOR ( elist ; expr ; elist ) stmt"); }
            ;

returnstmt:   RETURN ';'  { dlog("returnstmt -> RETURN;"); }
            | RETURN expr ';' { dlog("returnstmt -> RETURN expr;"); }
            ;

%%

int yyerror(std::string yaccProvidedMessage) {
    std::cout << yaccProvidedMessage << ": at line " << yylineno << ", before token: " << yytext << std::endl;
    std::cout << "INPUT NOT VALID" << std::endl;
    return 1;
}

int main(int argc, char** argv) {
    Init();
    
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

    GetSymbolTable().logSymbolTable();

    return 0;
}