#ifndef DATASTRUCTS_H
#define DATASTRUCTS_H

#include <string>
#include <stdlib.h>
#include <iostream>
#include <vector>

#define logtoken(token) std::cout << token->numline << ": #" << token->numToken << "\t\"" << token->content << "\"  " << token->type << std::endl

typedef struct alpha_token_t {
    unsigned int            numline;
    unsigned int            numToken;
    std::string             content;
    std::string             type;
    //struct alpha_token_t    *alpha_yylex;

    alpha_token_t();
    alpha_token_t(unsigned int, unsigned int, const std::string&, const std::string&);
} token;

token::alpha_token_t() = default;
token::alpha_token_t(unsigned int line, unsigned int num, const std::string& _content, const std::string& _type) : 
                    numline(line), numToken(num), content(_content), type(_type) {};

std::vector<token*> tokenList;

void logTokenList() {
    for(int i=0; i < tokenList.size(); i++)
        logtoken(tokenList.at(i));
}

#endif