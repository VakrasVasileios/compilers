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
%token LOCAL ID FUNCTION IF ELSE FOR WHILE BREAK CONTINUE NIL TRUE FALSE RETURN
%token <stringValue>    STRING
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

program:      stmt
            | stmt program
            ;

stmt:         expr';'
            | ifstmt
            | whilestmt
            | forstmt
            | returnstmt
            | BREAK';'
            | CONTINUE';'
            | block
            | funcdef
            | ';'
            ;

expr:         assignexpr
            | expr '+' expr
            | expr '-' expr
            | expr '*' expr
            | expr '/' expr
            | expr '%' expr
            | expr '>' expr
            | expr GEQL expr
            | expr '<' expr
            | expr LEQL expr
            | expr EQUAL expr
            | expr NOTEQUAL expr
            | expr AND expr
            | expr OR expr
            | term
            ;
            
term:         '('expr')'
            | '-'expr %prec UMINUS
            | NOT expr
            | PLUSPLUS lvalue
            | lvalue PLUSPLUS
            | MINUSMINUS lvalue
            | lvalue MINUSMINUS
            | primary
            ;

assignexpr:   lvalue'='expr
            ;

primary:      lvalue
            | call
            | objectdef
            | '('funcdef')'
            | const
            ;

lvalue:       ID
            | LOCAL ID
            | COLONCOLON ID
            | member
            ;

member:       lvalue'.'ID
            | lvalue'['expr']'
            | call'.'ID
            | call'['expr']'
            ;
            
call:        call'('elist')'
            | lvalue callsuffix
            | '('funcdef')''('elist')'
            ;
            
callsuffix:   normcall
            | methodcall
            ;

normcall:     '('elist')'
            ;

methodcall:   DOTDOT ID '('elist')'
            ;

multelist:    ','expr multelist
            |
            ;
elist:        '['expr multelist']'
            ;

objectdef:    '['elist']'
            | '['indexed']'
            ;

multindexed:  ','indexedelem multindexed
            |
            ;

indexed:      '['indexedelem multindexed']'
            ;

indexedelem:  '{' expr':'expr '}'
            ;

block:        '{'{ manager.IncreaseScope(); } stmt program '}' { manager.DecreaseScope(); }
            ;

funcdef:      FUNCTION '('idlist')' block
            | FUNCTION ID '('idlist')' block
            ;

const:        INTNUM | DOUBLENUM | STRING | NIL | TRUE | FALSE
            ;

multid:       ','ID multid
            |
            ;

idlist:       '['ID multid']'
            ;

ifstmt:       IF'('expr')' stmt elsestmt
            ;

elsestmt:     ELSE stmt
            |
            ;

whilestmt:    WHILE '(' expr ')' stmt
            ;

forstmt:      FOR '('elist';' expr';' elist')' stmt
            ;

returnstmt:   RETURN';'
            | RETURN expr';'
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