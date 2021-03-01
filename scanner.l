
%{
    #include <string>
    #include <stdlib.h>
    #include <iostream>

    #define log(str) std::cout << str << std::endl   

    #ifdef _cplusplus
        static int yyinput(void);
    #else
        static int input(void);
    #endif

    #define YY_DECL int yylex()
    using namespace std;
%}

%option header-file="./scanner.h"
%option noyywrap
/* enable c++ scanner class generation */
%option c++
%option nodefault
%option debug
%option yylineno

delim           [ \t\n]
ws              {delim}+
letter          [a-zA-Z]
digit           [0-9]
real            {digit}+.{digit}+
string          \"[^n"]*\"
underscore      [_]
id              {letter}({letter}|{digit}|{underscore})* 

number  {num1}|{num2}
num1            [-+]?{digit}+\.?([eE][-+]?{digit}+)?
num2            [-+]?{digit}*\.{digit}+([eE][-+]?{digit}+)?
%%
{id}        { log(yytext); } 


"<"         { log("less than"); }


{number} { int num = atoi(yytext); log(num); } 
{string} {cout << "string " << YYText() << '\n';}

<<EOF>>     { yyterminate(); }
%%

void
al(int argc, char** argv) {
    if (argc > 1) {
        if (!(yyin = fopen(argv[1], "r"))) {
            fprintf(stderr, "Cannot read file %s\n", argv[1]);
            return 1;
        }
    }
    else
        yyin = stdin;

    yylex();
}

int main(int argc, char** argv) {
    
    al(argc, argv);

    return 0;
}