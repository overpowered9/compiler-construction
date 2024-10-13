#include <iostream>
using namespace std;

// Node structure for the inner LinkedList
struct Node {
    string data;
    Node* next;

    Node(string value) : data(value), next(nullptr) {}
};

// LinkedList class for the inner LinkedList
class LinkedList {
public:
    Node* head;

    LinkedList() : head(nullptr) {}

    void insert(string value) {
        Node* newNode = new Node(value);
        if (head == nullptr) {
            head = newNode;
        } else {
            Node* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }

    void display() {
        Node* temp = head;
        while (temp != nullptr) {
            cout << temp->data << " ";
            temp = temp->next;
        }
    }
};

// RowNode structure for the outer LinkedList
struct RowNode {
    LinkedList* row;
    RowNode* down;

    RowNode() : row(new LinkedList()), down(nullptr) {}
};

// LinkedListOfLinkedLists class for the outer LinkedList
class LinkedListOfLinkedLists {
private:
    RowNode* head;

public:
    LinkedListOfLinkedLists() : head(nullptr) {}

    void insertRow() {
        RowNode* newRow = new RowNode();
        if (head == nullptr) {
            head = newRow;
        } else {
            RowNode* temp = head;
            while (temp->down != nullptr) {
                temp = temp->down;
            }
            temp->down = newRow;
        }
    }

    void insertInRow(int rowIndex, string value) {
        RowNode* temp = head;
        for (int i = 0; i < rowIndex; ++i) {
            if (temp == nullptr) return;
            temp = temp->down;
        }
        if (temp != nullptr) {
            temp->row->insert(value);
        }
    }

    void display() {
        RowNode* temp = head;
        while (temp != nullptr) {
            temp->row->display();
            cout << endl;
            temp = temp->down;
        }
    }
};

int main() {
    LinkedListOfLinkedLists list;
    list.insertRow();
    list.insertInRow(0, "A");
    list.insertInRow(0, "B");

    list.insertRow();
    list.insertInRow(1, "C");
    list.insertInRow(1, "D");

    list.display();

    return 0;
}