%{
    #include "parser.hpp"
    #include "../include/token.h"
    #include <string>
    #include <string.h>

    #ifdef _cplusplus
        static int yyinput(void);
    #else
        static int input(void);
    #endif

    #define YY_DECL int yylex(void)
    using namespace std;

    unsigned int tokenno = 0;
%}

/*Flex options*/
%option header-file="./scanner.hpp"
%option noyywrap
%option yylineno

/*Flex macros*/
IDENT           [a-zA-Z][a-zA-Z_0-9]*
INTCONST        [0-9]+
DOUBLECONST     {INTCONST}\.{INTCONST}
STRING          \"
PUNCTUATIONMARK [\{|\}|\[|\]|\(|\)|\;|\,|\:|\.]
SPACE           [\r \t\v]
NEWLINE         [\n]
OPERATOR        \+|\-|\*|\/|\=|\>|\<|\%|\^|\(|\)
SINGLELCOMMENT  \/\/
MULTILCOMMENT   [\/\*]
ILLEGALCHARS    [\$|\_|\!|\~|\?|\#]

/*Flex rules*/
%%


"or"                { return OR; }
"not"               { return NOT; }
"and"               { return AND; }
"local"             { return LOCAL; }
"true"              { return TRUE; }
"false"             { return FALSE; }
"nil"               { return NIL; }
"if"                { return IF; }
"else"              { return ELSE; }
"while"             { return WHILE; }
"for"               { return FOR; }
"function"          { return FUNCTION; }                       
"return"            { return RETURN; }
"break"             { return BREAK; }
"continue"          { return CONTINUE; }

{IDENT}             {
                        yylval.stringValue = strdup(yytext);
                        return ID;
                    }

{INTCONST}          {
                        int intConst = atoi(yytext);
                        yylval.intValue = intConst;
                        return INTNUM;
                        
                    }

{DOUBLECONST}       {
                        double doubleConst = atof(yytext);
                        yylval.doubleValue = doubleConst;
                        return DOUBLENUM;
                    }

{STRING}            {
                        string str;
                        int line = yylineno;
                        int c = yyinput();

                        while(c != '\"') {
                            if (c == EOF || c == NULL) {
                                fprintf(stderr, "\tUnclosed string at line: %d\n", line);
                                return 1;
                            }
                            if (c == '\\') {
                                c = yyinput();
                                switch(c) {
                                    case 'n':
                                        str += '\n'; 
                                        break;
                                    case 't':
                                        str += '\t';
                                        break;
                                    case 'r':
                                        str += '\r';
                                        break;
                                    case 'v':
                                        str += '\v';
                                        break;
                                    case '\\':
                                        str += '\\';
                                        break;
                                    case '\"':
                                        str += '\"';
                                        break;
                                    default:
                                        fprintf(stderr, "\tIllegal escape character at line: %d\n", line);
                                        return 1;
                                }
                            }
                            else
                                str += (char)c;
                            c = yyinput();
                        }

                        yylval.stringValue = strdup(const_cast<char*>(str.c_str()));
                        return STRING;
                    }                    

{PUNCTUATIONMARK}   {
                        return *yytext;
                    }
"::"                { return COLONCOLON; }
".."                { return DOTDOT;     }

{SPACE}             {}

{NEWLINE}           {}

{OPERATOR}          {
                        return *yytext;
                    }
                    
">="                { return GEQL;       }
"<="                { return LEQL;       }
"=="                { return EQUAL;      }
"!="                { return NOTEQUAL;   }
"++"                { return PLUSPLUS;   }
"--"                { return MINUSMINUS; }

{SINGLELCOMMENT}    {
                        int c = 1;

                        while(c != '\n') {
                            c = yyinput();
                            if (c == EOF || c == NULL)   break;
                        }
                    }

{MULTILCOMMENT}     {
                        int depth = 1;
                        int line = yylineno;
                        int c = yyinput();
                        int count = 1;

                        while(1) {
                            if (c == EOF || c == NULL) {
                                fprintf(stderr, "\tUnclosed multiline comment at line: %d\n", line);
                                return 1;
                            }
                            if(c == '/') {
                                c = yyinput();
                                if (c == '*') {
                                    ++count;
                                    ++depth;
                                    c = yyinput();
                                }
                            }
                            else if(c == '*') {
                                c = yyinput();
                                if(c == '/'){
                                    --depth;
                                    if (depth == 0) break;
                                    c = yyinput();
                                }
                            }
                            else {
                                c = yyinput();
                            }
                        }
                    }
                    
{ILLEGALCHARS}      {
                        fprintf(stderr, "\tIllegal character at line: %d\n", yylineno);
                        return 1;
                    }

<<EOF>>             { return 0; }

%%
/*
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

    logTokenList();

    return 0;
}
*/