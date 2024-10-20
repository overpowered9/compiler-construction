#ifndef PROD_H
#define PROD_H

#include "Rule.h"
#include <string>

class Prod
{
public:
    std::string nt;
    Rule *ruleHead;
    Rule *ruleTail;
    Prod *prev;
    Prod *next;

    Prod(const std::string t);
    void addRule(const std::string s);
    ~Prod();
};

#endif
