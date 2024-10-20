#include "Prod.h"

Prod::Prod(const std::string t) : nt(t), ruleHead(nullptr), ruleTail(nullptr), prev(nullptr), next(nullptr) {}

void Prod::addRule(const std::string s)
{
    Rule *newRule = new Rule();
    std::string currentToken;

    for (size_t i = 0; i < s.length(); ++i)
    {
        char c = s[i];
        if (c == ' ')
        {
            newRule->addToken(currentToken);
            currentToken.clear();
        }
        else
        {
            currentToken.push_back(c);
        }
    }

    if (!currentToken.empty())
    {
        newRule->addToken(currentToken);
    }

    if (!ruleHead)
    {
        ruleHead = ruleTail = newRule;
    }
    else
    {
        newRule->prev = ruleTail;
        ruleTail->next = newRule;
        ruleTail = newRule;
    }
}

Prod::~Prod()
{
    Rule *currentRule = ruleHead;
    while (currentRule)
    {
        Rule *tempRule = currentRule;
        currentRule = currentRule->next;
        delete tempRule;
    }
}
