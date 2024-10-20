#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include "Prod.h"

class DoublyLinkedList
{
public:
    DoublyLinkedList();
    void insertBack(Prod *p);
    Prod *getHead() const;
    ~DoublyLinkedList();

private:
    Prod *head;
    Prod *tail;
};

#endif
