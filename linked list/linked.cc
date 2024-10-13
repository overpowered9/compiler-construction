#include <iostream>
#include <sstream>
using namespace std;

struct Node{
    string data;
    Node* next;

};
class LinkedList {
private:
    Node* head;

public:
    // Constructor to initialize the list
    LinkedList() {
        head = nullptr;
    }
    void insertathead(string Value){
         Node* newNode = new Node();
        newNode->data = Value;
        
        if (head!=nullptr)
        {
            newNode->next=head;
            head=newNode;
        }else{
            head=newNode;
            newNode->next = nullptr;
        }
        
        

    }
    void display(){
        Node* temp=head;
        while(temp->next!=nullptr){
            cout << temp->data <<"->";
            temp=temp->next;
        }
        cout << temp->data;
    }

};
int main(){
    LinkedList ll;
     string input;
     cout << "Enter a string: ";
    getline(cin,input);
stringstream ss(input);
string word;
  while (ss >> word) {
        ll.insertathead(word);
    }
 
    ll.display();

}