#include <iostream>
using namespace std;

// Node structure
struct Node {
    int data;
    Node* next;
};

// LinkedList class
class LinkedList {
private:
    Node* head;

public:
    // Constructor to initialize the list
    LinkedList() {
        head = nullptr;
    }

    // Function to insert a new node at the end of the list
    void insert(int value) {
        Node* newNode = new Node();
        newNode->data = value;
        newNode->next = nullptr;

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

    // Function to display the linked list
    void display() {
        Node* temp = head;
        while (temp != nullptr) {
            cout << temp->data << " -> ";
            temp = temp->next;
        }
        cout << "NULL" << endl;
    }

    // Function to delete a node by its value
    void deleteNode(int value) {
        Node* temp = head;
        Node* prev = nullptr;

        // If the head node itself holds the key
        if (temp != nullptr && temp->data == value) {
            head = temp->next; // Change head
            delete temp;       // Free memory
            return;
        }

        // Search for the key to be deleted
        while (temp != nullptr && temp->data != value) {
            prev = temp;
            temp = temp->next;
        }

        // If the key was not present in the list
        if (temp == nullptr) return;

        // Unlink the node from the linked list
        prev->next = temp->next;
        delete temp;
    }

    // Destructor to free memory
    ~LinkedList() {
        Node* current = head;
        Node* nextNode;
        while (current != nullptr) {
            nextNode = current->next;
            delete current;
            current = nextNode;
        }
        head = nullptr;
    }
};

int main() {
    LinkedList list;
    list.insert(10);
    list.insert(20);
    list.insert(30);
    list.insert(40);

    cout << "Linked List: ";
    list.display();

    cout << "Deleting node with value 20" << endl;
    list.deleteNode(20);

    cout << "Updated Linked List: ";
    list.display();

    return 0;
}
