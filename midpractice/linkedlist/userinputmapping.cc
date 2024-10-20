#include <iostream>
#include <sstream>
using namespace std;

// Node structure for LinkedList
struct Node {
    string word;
    Node* next;
    
    // Constructor to create a new node
    Node(string w) : word(w), next(nullptr) {}
};

// LinkedList class to store words
class LinkedList {
private:
    Node* head;
    
public:
    LinkedList() : head(nullptr) {}
    
    // Function to insert a word into the LinkedList
    void insert(string word) {
        Node* newNode = new Node(word);
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
    
    // Function to display the words in the LinkedList
    void display() {
        Node* temp = head;
        while (temp->next != nullptr) {
            cout << temp->word << " ->";
            temp = temp->next;
        }
        cout << temp->word;
        cout << endl;
    }
    
    // Destructor to free the memory
    ~LinkedList() {
        Node* current = head;
        while (current != nullptr) {
            Node* nextNode = current->next;
            delete current;
            current = nextNode;
        }
    }
};

int main() {
    LinkedList list;
    string sentence;
    
    cout << "Enter a sentence: ";
    getline(cin, sentence);
    
    // Breaking sentence into words
    stringstream ss(sentence);
    string word;
    
    while (ss >> word) {
        list.insert(word);
    }
    
    // Display the words stored in the LinkedList
    cout << "Words in the LinkedList: ";
    list.display();
    
    return 0;
}
