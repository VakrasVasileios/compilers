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
                                cout << line << ": Unclosed string error!" << endl;
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
                                        str += '\\';
                                        str += c;
                                        break;
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
                        
                        ++tokenno;
                        tokenList.push_back(new token(line, tokenno, "", "MULTILCOMMENT"));

                        while(1) {
                            if (c == EOF || c == NULL) {
                                cout << line << ": Unclosed multiline comment error!" <<endl;
                                return 1;
                            }
                            if(c == '/') {
                                c = yyinput();
                                if (c == '*') {
                                    ++tokenno;
                                    tokenList.push_back(new token(yylineno, tokenno, "", "NESTEDCOMMENT"));
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
                        /* TO BE enabled in phase 2
                        cout << yylineno << ": Undefined character error!" << endl;
                        return 1;
                        */
                        ++tokenno;
                        tokenList.push_back(new token(yylineno, tokenno, yytext, "ILLEGALCHARS"));
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