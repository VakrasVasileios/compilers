%{
    #include "dataStructs.h"

    #ifdef _cplusplus
        static int yyinput(void);
    #else
        static int input(void);
    #endif

    #define YY_DECL int alpha_yylex(void* yylval)
    using namespace std;

    unsigned int tokenno = 0;
%}

/*Flex options*/
%option header-file="./scanner.h"
%option noyywrap
%option yylineno

/*Flex macros*/
KEYWORD         (if|else|while|for|function|return|break|continue|and|not|or|local|true|false|nil)
IDENT           [a-zA-Z][a-zA-Z_0-9]*
INTCONST        [0-9]+
DOUBLECONST     {INTCONST}.{INTCONST}
STRING          \"
PUNCTUATIONMARK (\{|\}|\[|\]|\(|\)|\;|\,|\:|\:\:|\.|\.\.)
SPACE           [\r \t\v]
NEWLINE         [\n]
OPERATOR        (\+|\-|\*|\/|\=|\>|\<|\>\=|\<\=|\&|\||\%|\!\=|\^|\(|\)|\=\=|\+\+|\-\-)
SINGLELCOMMENT  \/\/
MULTILCOMMENT   \/\*

/*Flex rules*/
%%

{KEYWORD}           {
                        ++tokenno;
                        tokenList.push_back(new token(yylineno, tokenno, string(yytext), "KEYWORD"));
                    }

{IDENT}             {
                        ++tokenno;
                        tokenList.push_back(new token(yylineno, tokenno, string(yytext), "IDENT"));
                    }

{INTCONST}          {
                        ++tokenno;
                        tokenList.push_back(new token(yylineno, tokenno, string(yytext), "INTCONST"));
                    }

{DOUBLECONST}       {
                        ++tokenno;
                        tokenList.push_back(new token(yylineno, tokenno, string(yytext), "DOUBLECONST"));
                    }

{STRING}            {
                        string str;
                        int c = yyinput();

                        while(c != '\"' && c != EOF) {
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
                                    default:
                                        str += '\\';
                                        str += c;
                                        break;
                                }
                            }
                            else if (c == '\n')
                                ++yylineno;
                            else
                                str += (char)c;
                            c = yyinput();
                        }

                        ++tokenno;
                        
                        tokenList.push_back(new token(yylineno, tokenno, str, "STRING"));
                    }                    

{PUNCTUATIONMARK}   {
                        ++tokenno;
                        tokenList.push_back(new token(yylineno, tokenno, string(yytext), "PUNCTUATIONMARK"));   
                    }

{SPACE}             {}

{NEWLINE}           {}

{OPERATOR}          {
                        ++tokenno;
                        tokenList.push_back(new token(yylineno, tokenno, string(yytext), "OPERATOR"));
                    }

{SINGLELCOMMENT}    {
                        string comment;
                        int c = yyinput();

                        while(c != '\n' && c != EOF) {
                            comment += (char)c;
                            c = yyinput();
                        }

                        ++tokenno;

                        tokenList.push_back(new token(yylineno, tokenno, comment, "SINGLELCOMMENT"));
                    }

{MULTILCOMMENT}     {
                        string comment;
                        int c = yyinput();
                        int depth = 1;

                        while(c != EOF) {       //TODO
                            if(c == '/') {
                                c = yyinput();
                                switch(c) {
                                    case '*' :
                                        comment += '/';
                                        comment += (char)c;
                                        ++depth;
                                        break;
                                    default:
                                        comment += '/';
                                        comment += (char)c;
                                        break;
                                }
                            }
                            else if(c == '*') {
                                c = yyinput();
                                switch(c) {
                                    case '/':
                                        --depth;
                                        if (depth != 0) {
                                            comment += '*';
                                            comment += '/';
                                        }
                                        break;
                                    default:
                                        comment += (char)c;
                                        break;    
                                }
                            }
                            else if (c == '\n') {
                                ++yylineno;
                            }
                            else {
                                comment += (char)c;
                            }
                            
                            if (depth == 0)
                                break;
                            c = yyinput();
                        }

                        ++tokenno;
                        tokenList.push_back(new token(yylineno, tokenno, comment, "MULTILCOMMENT"));
                    }                  

<<EOF>>             { return 0; }

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

    logTokenList();

    return 0;
}