#ifndef TOKEN_H
#define TOKEN_H

#include <string>

class Token
{
public:
    std::string t;
    Token *next;
    Token *prev;

    Token(const std::string t);
};

#endif // TOKEN_H
