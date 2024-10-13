#include <iostream>
using namespace std;

struct Node {
    string data;
    Node* next; // Pointer to the next node in the same row
    Node* down; // Pointer to the next node in the same column

    Node(string value) : data(value), next(nullptr), down(nullptr) {}
};

class LinkedList {
private:
    Node* head;

public:
    // Constructor to initialize the list
    LinkedList() {
        head = nullptr;
    }

    // Insert a node at the end of a specific row
    void insertInRow(Node* rowHead, string value) {
        Node* newNode = new Node(value);
        if (rowHead == nullptr) {
            head = newNode;
        } else {
            Node* temp = rowHead;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }

    // Insert a node at the end of a specific column
    void insertInColumn(Node* colHead, string value) {
        Node* newNode = new Node(value);
        if (colHead == nullptr) {
            head = newNode;
        } else {
            Node* temp = colHead;
            while (temp->down != nullptr) {
                temp = temp->down;
            }
            temp->down = newNode;
        }
    }

    // Insert a node at a specific position (row, col)
    void insertAtPosition(int row, int col, string value) {
        if (head == nullptr) {
            head = new Node(value);
            return;
        }

        Node* rowHead = head;
        for (int i = 0; i < row; ++i) {
            if (rowHead->down == nullptr) {
                rowHead->down = new Node("");
            }
            rowHead = rowHead->down;
        }

        Node* colHead = rowHead;
        for (int j = 0; j < col; ++j) {
            if (colHead->next == nullptr) {
                colHead->next = new Node("");
            }
            colHead = colHead->next;
        }

        colHead->data = value;
    }

    // Display the 2D LinkedList
    void display() {
        Node* rowHead = head;
        while (rowHead != nullptr) {
            Node* colHead = rowHead;
            while (colHead != nullptr) {
                cout << colHead->data << " ";
                colHead = colHead->next;
            }
            cout << endl;
            rowHead = rowHead->down;
        }
    }
};

int main() {
    LinkedList list;
    list.insertAtPosition(0, 0, "A");
    list.insertAtPosition(0, 1, "B");
    list.insertAtPosition(1, 0, "C");
    list.insertAtPosition(1, 1, "D");

    list.display();

    return 0;
}