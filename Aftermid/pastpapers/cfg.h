#ifndef CFG_H
#define CFG_H

#include <iostream>
#include <string>

class Token {
public:
    std::string data;
    Token* next;
    Token* prev;

    Token(std::string data);
};

class Rule {
public:
    Rule* next;
    Rule* prev;
    Token* tokenHead;
    Token* tokenTail;

    Rule();
    void addToken(std::string tokenData);
    ~Rule();
};

class Prod {
public:
    std::string nt;
    Prod* next;
    Prod* prev;
    Rule* rulehead;
    Rule* ruletail;

    Prod(std::string t);
    void addRule(Rule* newRule);
    ~Prod();
};

class DoublyLinkedList {
public:
    Prod* prodhead;
    Prod* prodtail;

    DoublyLinkedList();
    void insertattail(Prod* p);
    Prod* gethead();
    ~DoublyLinkedList();
};

// Function to remove null productions
void removeNullProductions(DoublyLinkedList& prodList);

#endif
