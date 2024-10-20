#ifndef RULE_H
#define RULE_H

#include "Token.h"

class Rule
{
public:
    Rule *prev;
    Rule *next;

    Token *tokenHead;
    Token *tokenTail;

    Rule();
    void addToken(const std::string t);
    ~Rule();
};

#endif
