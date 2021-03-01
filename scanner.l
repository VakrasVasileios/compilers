%{
    #include <string>
    #include <stdlib.h>
    #include <iostream>  

    #ifdef _cplusplus
        static int yyinput(void);
    #else
        static int input(void);
    #endif

    #define YY_DECL int alpha_yylex(void* yylval)
    using namespace std;

    #define logtoken(tok) cout << yylineno << ": #" << tokenno << "\t\"" << yytext << "\"  " << tok << endl
    //#define logtoken(tok) cout << tok->numline << ": #" << tok->numToken << "\t\"" << tok->content << "\"  " << tok->type << endl
 
    unsigned int tokenno = 0;

    struct alpha_token_t {
        unsigned int            numline;
        unsigned int            numToken;
        string                  content;
        string                  type;
        struct alpha_token_t    *alpha_yylex;
    };
%}

/*Flex options*/
%option header-file="./scanner.h"
%option noyywrap
%option yylineno

/*Flex macros*/
IDENT           [a-zA-Z][a-zA-Z_0-9]*
INTCONST        [0-9]+
DOUBLECONST     {INTCONST}.{INTCONST}
SPACE           [\r\n \t\v]

/*Flex rules*/
%%

{IDENT}         {   ++tokenno;              //TODO
                    logtoken("IDENT");      
                }

{INTCONST}      {                           //TODO
                    ++tokenno;
                    logtoken("INTCONST");
                }

{DOUBLECONST}   {                           //TODO
                    ++tokenno;
                    logtoken("DOUBLECONST");
                }
                    
"/*"            {
    
}

"\""    {

}

%%


void
al(int argc, char** argv) {
    if (argc > 1) {
        if (!(yyin = fopen(argv[1], "r"))) {
            fprintf(stderr, "Cannot read file %s\n", argv[1]);
            return;
        }
    }
    else
        yyin = stdin;

    alpha_yylex(NULL);
}

int main(int argc, char** argv) {
    
    al(argc, argv);

    return 0;
}