#include "cfg.h"

// Token implementation
Token::Token(std::string data) : data(data), next(nullptr), prev(nullptr) {}

// Rule implementation
Rule::Rule() : next(nullptr), prev(nullptr), tokenHead(nullptr), tokenTail(nullptr) {}

void Rule::addToken(std::string tokenData) {
    Token* newToken = new Token(tokenData);
    if (!tokenHead) {
        tokenHead = tokenTail = newToken;
    } else {
        tokenTail->next = newToken;
        newToken->prev = tokenTail;
        tokenTail = newToken;
    }
}

Rule::~Rule() {
    Token* current = tokenHead;
    while (current) {
        Token* temp = current;
        current = current->next;
        delete temp;
    }
}

// Prod implementation
Prod::Prod(std::string t) : nt(t), next(nullptr), prev(nullptr), rulehead(nullptr), ruletail(nullptr) {}

void Prod::addRule(Rule* newRule) {
    if (!rulehead) {
        rulehead = ruletail = newRule;
    } else {
        ruletail->next = newRule;
        newRule->prev = ruletail;
        ruletail = newRule;
    }
}

Prod::~Prod() {
    Rule* current = rulehead;
    while (current) {
        Rule* temp = current;
        current = current->next;
        delete temp;
    }
}

// DoublyLinkedList implementation
DoublyLinkedList::DoublyLinkedList() : prodhead(nullptr), prodtail(nullptr) {}

void DoublyLinkedList::insertattail(Prod* p) {
    if (!prodhead) {
        prodhead = prodtail = p;
    } else {
        prodtail->next = p;
        p->prev = prodtail;
        prodtail = p;
    }
}

Prod* DoublyLinkedList::gethead() {
    return prodhead;
}

DoublyLinkedList::~DoublyLinkedList() {
    Prod* current = prodhead;
    while (current) {
        Prod* temp = current;
        current = current->next;
        delete temp;
    }
}

// Remove null productions
