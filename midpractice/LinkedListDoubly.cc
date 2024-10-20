#include <iostream>
using namespace std;

struct Node {
    string data;
    Node* next;
    Node* prev;

    Node(string value) : data(value), next(nullptr), prev(nullptr) {}
};

class LinkedList {
private:
    Node* head;
    Node* tail;

public:
    LinkedList() : head(nullptr), tail(nullptr) {}

    void insertAtHead(string data) {
        Node* newNode = new Node(data);
        if (head != nullptr) {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        } else {
            head = newNode;
            tail = newNode;
        }
    }

    void insertAtTail(string data) {
        Node* newNode = new Node(data);
        if (tail != nullptr) {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        } else {
            head = newNode;
            tail = newNode;
        }
    }

    void insertAtPosition(int position, string data) {
        if (position == 0) {
            insertAtHead(data);
            return;
        }

        Node* newNode = new Node(data);
        Node* temp = head;
        for (int i = 0; i < position - 1 && temp != nullptr; ++i) {
            temp = temp->next;
        }

        if (temp == nullptr || temp->next == nullptr) {
            insertAtTail(data);
        } else {
            newNode->next = temp->next;
            newNode->prev = temp;
            temp->next->prev = newNode;
            temp->next = newNode;
        }
    }

    void deleteAtHead() {
        if (head == nullptr) return;

        Node* temp = head;
        head = head->next;
        if (head != nullptr) {
            head->prev = nullptr;
        } else {
            tail = nullptr;
        }
        delete temp;
    }

    void deleteAtTail() {
        if (tail == nullptr) return;

        Node* temp = tail;
        tail = tail->prev;
        if (tail != nullptr) {
            tail->next = nullptr;
        } else {
            head = nullptr;
        }
        delete temp;
    }

    void deleteAtPosition(int position) {
        if (position == 0) {
            deleteAtHead();
            return;
        }

        Node* temp = head;
        for (int i = 0; i < position && temp != nullptr; ++i) {
            temp = temp->next;
        }

        if (temp == nullptr) return;

        if (temp->prev != nullptr) {
            temp->prev->next = temp->next;
        }

        if (temp->next != nullptr) {
            temp->next->prev = temp->prev;
        }

        delete temp;
    }

    void display() {
        Node* temp = head;
        while (temp != nullptr) {
            cout << temp->data << " ";
            temp = temp->next;
        }
        cout << endl;
    }
};

int main() {
    LinkedList ll;
    ll.insertAtHead("10");
    ll.insertAtHead("20");
    ll.insertAtHead("30");
    ll.insertAtHead("40");
    ll.insertAtTail("50");
    ll.insertAtPosition(2, "25");
    ll.display();

    ll.deleteAtHead();
    ll.display();

    ll.deleteAtTail();
    ll.display();

    ll.deleteAtPosition(2);
    ll.display();

    return 0;
}