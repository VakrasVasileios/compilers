%{
    #include <stdio.h>
    #include <string>
    #include <iostream>
    #include "UnionManager.h"
    
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
%token local id function if else for while break continue nil true false return
%token <stringValue>    STRING
%token <intValue>       INTNUM
%token <doubleValue>    DOUBLENUM

%right      '='
%left       or
%left       and
%nonassoc   EQUAL NOTEQUAL
%nonassoc   '>' GEQL '<' LEQL
%left       '-' '+'
%left       '*' '/' '%'
%nonassoc   UMINUS
%right      not MINUSMINUS PLUSPLUS 
%left       '.' DOTDOT
%left       '[' ']'
%left       '(' ')'

%type <stringValue>     id

%%

program:      stmt
            | stmt program
            ;

stmt:         expr';'
            | ifstmt
            | whilestmt
            | forstmt
            | returnstmt
            | break';'
            | continue';'
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
            | expr and expr
            | expr or expr
            | term 
            ;
            
term:         '('expr')'
            | '-'expr %prec UMINUS
            | not expr
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

lvalue:       id
            | local id
            | COLONCOLON id
            | member
            ;

member:       lvalue'.'id
            | lvalue'['expr']'
            | call'.'id
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

methodcall:   DOTDOT id '('elist')'
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

block:        '{' stmt program '}'
            ;

funcdef:      function '('idlist')' block
            | function id '('idlist')' block
            ;

const:        INTNUM | DOUBLENUM | STRING | nil | true | false
            ;

multid:       ','id multid
            |
            ;

idlist:       '['id multid']'
            ;

elsestmt:     else stmt
            |
            ;

ifstmt:       if'('expr')' stmt elsestmt
            ;

whilestmt:    while '(' expr ')' stmt
            ;

forstmt:      for '('elist';' expr';' elist')' stmt
            ;

returnstmt:   return';'
            | return expr';'
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