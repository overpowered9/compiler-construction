#include "Rule.h"

Rule::Rule() : prev(nullptr), next(nullptr), tokenHead(nullptr), tokenTail(nullptr) {}

void Rule::addToken(const std::string t)
{
    Token *newToken = new Token(t);
    if (!tokenHead)
    {
        tokenHead = tokenTail = newToken;
    }
    else
    {
        newToken->prev = tokenTail;
        tokenTail->next = newToken;
        tokenTail = newToken;
    }
}

Rule::~Rule()
{
    Token *currentToken = tokenHead;
    while (currentToken)
    {
        Token *tempToken = currentToken;
        currentToken = currentToken->next;
        delete tempToken;
    }
}
