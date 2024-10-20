#include "DoublyLinkedList.h"
DoublyLinkedList::DoublyLinkedList() : head(nullptr), tail(nullptr) {}

void DoublyLinkedList::insertBack(Prod *p)
{
    if (!head)
    {
        head = tail = p;
    }
    else
    {
        p->prev = tail;
        tail->next = p;
        tail = p;
    }
}

Prod *DoublyLinkedList::getHead() const
{
    return head;
}

DoublyLinkedList::~DoublyLinkedList()
{
    Prod *current = head;
    while (current)
    {
        Prod *temp = current;
        current = current->next;
        delete temp;
    }
}
