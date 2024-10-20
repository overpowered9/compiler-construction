#include <iostream>
#include <string>
using namespace std;

class Token {
public:
    string data;      // Public so it can be accessed outside the class
    Token* next;      // Pointer to the next Token in the linked list
    Token* prev;      // Pointer to the previous Token in the linked list

    // Constructor to initialize the data and set next and prev pointers to nullptr
    Token(string String) : data(String), next(nullptr), prev(nullptr) {}
};

int main() {

}
