%{
    #include <stdio.h>
    #include <string>
    #include <iostream>
    #include "include/UnionManager.h"
    
    int yyerror(std::string yaccProvidedMessage);
    int yylex(void);

    extern int yylineno;
    extern char* yytext;
    extern FILE* yyin;
    UnionManager manager;

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

program:      stmt                  { std::cout << "stmt" << std::endl;}
            | stmt program          { std::cout << "stmt program" << std::endl;}
            ;

stmt:         expr';'               { std::cout << "expr;" << std::endl;}
            | ifstmt                { std::cout << "ifstmt"<< std::endl;}
            | whilestmt             { std::cout << "whilestmt" << std::endl;}
            | forstmt               { std::cout << "forstmt" << std::endl;}
            | returnstmt            { std::cout << "returnstmt" << std::endl;}
            | BREAK';'              { std::cout << "break;" << std::endl;}
            | CONTINUE';'           { std::cout << "continue;" << std::endl;}
            | block                 { std::cout << "block" << std::endl;}
            | funcdef               { std::cout << "funcdef" << std::endl;}
            | ';'                   { std::cout << ";" << std::endl;}
            ;

expr:         assignexpr            { std::cout << "assignexpr" << std::endl;}
            | expr '+' expr         { std::cout << "expr + expr" << std::endl; }
            | expr '-' expr         { std::cout << "expr - expr" << std::endl; }
            | expr '*' expr         { std::cout << "expr * expr" << std::endl; }
            | expr '/' expr         { std::cout << "expr / expr" << std::endl; }
            | expr '%' expr         { std::cout << "expr % expr" << std::endl; }
            | expr '>' expr         { std::cout << "expr > expr" << std::endl; }
            | expr GEQL expr        { std::cout << "expr >= expr" << std::endl;}
            | expr '<' expr         { std::cout << "expr + expr" << std::endl; }
            | expr LEQL expr        { std::cout << "expr <= expr" << std::endl;}
            | expr EQUAL expr       { std::cout << "expr == expr" << std::endl;}
            | expr NOTEQUAL expr    { std::cout << "expr != expr" << std::endl;}
            | expr AND expr         { std::cout << "expr and expr" << std::endl; }
            | expr OR expr          { std::cout << "expr or expr" << std::endl; }
            | term                  { std::cout << "term" << std::endl;}
            ;            
term:         '('expr')'            { std::cout << "(expr)" << std::endl; }
            | '-'expr %prec UMINUS  { std::cout << "-expr %prec UMINUS" << std::endl; }
            | NOT expr              { std::cout << "not expr" << std::endl; }
            | PLUSPLUS lvalue       { std::cout << "++ lvalue" << std::endl; }
            | lvalue PLUSPLUS       { std::cout << "lvalue ++" << std::endl; }
            | MINUSMINUS lvalue     { std::cout << "-- lvaule" << std::endl; }
            | lvalue MINUSMINUS     { std::cout << "lvalue --" << std::endl; }
            | primary               { std::cout << "primary" << std::endl; }
            ;

assignexpr:   lvalue'='expr         { std::cout << "lvalue = expr" << std::endl; }
            ;

primary:      lvalue                { std::cout << "lvalue" << std::endl; }
            | call                  { std::cout << "call" << std::endl; }
            | objectdef             { std::cout << "objectdef" << std::endl; }
            | '('funcdef')'         { std::cout << "(funcdef)" << std::endl; }
            | const                 { std::cout << "const" << std::endl; }
            ;

lvalue:       ID                    { std::cout << "id" << std::endl; }
            | LOCAL ID              { std::cout << "local id" << std::endl; }
            | COLONCOLON ID         { std::cout << "::id" << std::endl; }
            | member                { std::cout << "member" << std::endl; }
            ;   

member:       lvalue'.'ID           { std::cout << "lvalue.id" << std::endl; }
            | lvalue '['expr']'     { std::cout << "lvalue expr" << std::endl; }
            | call'.'ID             { std::cout << "call.id" << std::endl; }
            | call '['expr']'       { std::cout << "call expr" << std::endl; }
            ;
            
call:        call'('elist')'        { std::cout << "call(elist)" << std::endl; }
            | lvalue callsuffix     { std::cout << "lvalue callsuffix" << std::endl; }
            | '('funcdef')''('elist')'  { std::cout << "(funcdef)(elist)" << std::endl; }
            ;
            
callsuffix:   normcall              { std::cout << "normcall" << std::endl; }
            | methodcall            { std::cout << "methodcall" << std::endl; }
            ;

normcall:     '('elist')'           { std::cout << "(elist)" << std::endl; }
            ;

methodcall:   DOTDOT ID '('elist')' { std::cout << "..id (elist)" << std::endl; }
            ;

multelist:    ','expr multelist     { std::cout << ",expr multelist" << std::endl; }
            |
            ;
elist:        expr multelist        { std::cout << "expr multelist" << std::endl; }
            |
            ;

objectdef:    '[' elist ']'         { std::cout << "[elist]" << std::endl; }
            | '[' indexed ']'       { std::cout << "[indexed]" << std::endl; }
            ;

multindexed:  ','indexedelem multindexed { std::cout << ", indexedelem multidexed" << std::endl; }
            |
            ;

indexed:      indexedelem multindexed    { std::cout << "indexedelem multidexed" << std::endl; }
            ;

indexedelem:  '{' expr':'expr '}'   { std::cout << "{ expr : expr }" << std::endl; }
            ;

block:        '{' program '}'  { std::cout << "{ program }" << std::endl; }
            | '{' '}'          { std::cout << "{}" << std::endl;}
            ;

funcdef:      FUNCTION '('idlist')' block { std::cout << "function (idlist) block " << std::endl; }
            | FUNCTION ID '('idlist')' block { std::cout << "function id (idlist) block" << std::endl; }
            ;

const:        INTNUM                { std::cout << "INTNUM" << std::endl; }
            | DOUBLENUM             { std::cout << "DOUBLENUM" << std::endl;}
            | STRING                { std::cout << "STRING" << std::endl; }
            | NIL                   { std::cout << "NIL" << std::endl; }
            | TRUE                  { std::cout << "TRUE" << std::endl; }
            | FALSE                 { std::cout << "FALSE" << std::endl; }
            ;

multid:       ','ID multid          { std::cout << ", id multid" << std::endl;}
            |
            ;

idlist:       ID multid             { std::cout << "id multid" << std::endl; }
            |
            ;

ifstmt:       IF'('expr')' stmt elsestmt { std::cout << "if (expr) stmt elsestmt" << std::endl; }
            ;

elsestmt:     ELSE stmt { std::cout << "else stmt" << std::endl; }
            |
            ;

whilestmt:    WHILE '(' expr ')' stmt { std::cout << "WHILE (expr) stmt" << std::endl; }
            ;

forstmt:      FOR '('elist';' expr';' elist')' stmt { std::cout << "FOR ( elist ; expr ; elist ) stmt" << std::endl; }
            ;

returnstmt:   RETURN';'  { std::cout << "RETURN;" << std::endl; }
            | RETURN expr';' { std::cout << "RETURN expr;" << std::endl; }
            ;

%%

int yyerror(std::string yaccProvidedMessage) {
    std::cout << yaccProvidedMessage << ": at line " << yylineno << ", before token: " << yytext << std::endl;
    std::cout << "INPUT NOT VALID" << std::endl;
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

    yyparse();

    return 0;
}