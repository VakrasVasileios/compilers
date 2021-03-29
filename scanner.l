%{
    #include "parser.hpp"
    #include "dataStructs.h"

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
KEYWORD         (if|else|while|for|function|return|break|continue|and|not|or|local|true|false|nil)
IDENT           [a-zA-Z][a-zA-Z_0-9]*
INTCONST        [0-9]+
DOUBLECONST     {INTCONST}\.{INTCONST}
STRING          \"
PUNCTUATIONMARK (\{|\}|\[|\]|\(|\)|\;|\,|\:|\:\:|\.|\.\.)
SPACE           [\r \t\v]
NEWLINE         [\n]
OPERATOR        (\+|\-|\*|\/|\=|\>|\<|\>\=|\<\=|\%|\!\=|\^|\(|\)|\=\=|\+\+|\-\-)
SINGLELCOMMENT  \/\/
MULTILCOMMENT   (\/\*)
ILLEGALCHARS    (\$|\_|\!|\~|\?|\#)

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

                        ++tokenno;
                        tokenList.push_back(new token(line, tokenno, str, "STRING"));
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
                        int c = 1;
                        
                        ++tokenno;
                        tokenList.push_back(new token(yylineno, tokenno, "", "SINGLELCOMMENT"));

                        while(c != '\n' && c != EOF) {
                            c = yyinput();
                        }
                    }

{MULTILCOMMENT}     {
                        int depth = 1;
                        int line = yylineno;
                        int c = yyinput();
                        int count = 1;
                        
                        ++tokenno;
                        tokenList.push_back(new token(line, tokenno, "", "MULTILCOMMENT"));

                        while(1) {
                            if (c == EOF || c == NULL) {
                                fprintf(stderr, "\tUnclosed multiline comment at line: %d\n", line);
                                // Remove the comment, along with it's nested ones from the list
                                auto iter = tokenList.end();
                                for (; count > 0; count--) {
                                    delete *(--iter);
                                    tokenList.erase(iter);
                                }
                                return 1;
                            }
                            if(c == '/') {
                                c = yyinput();
                                if (c == '*') {
                                    ++tokenno;
                                    tokenList.push_back(new token(yylineno, tokenno, "", "NESTEDCOMMENT"));
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