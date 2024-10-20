#include <iostream>
#include <string>
using namespace std;

class Token {
public:
    string data;  // Public so it can be accessed outside the class
    Token* next;  // Pointer to the next Token in the linked list
    Token* prev;  // Pointer to the previous Token in the linked list

    Token(string data) : data(data), next(nullptr), prev(nullptr) {}
};

class Rule {

public:
    Rule* next;
    Rule* prev;

    Token* tokenHead;
    Token* tokenTail;

    Rule() : next(nullptr), prev(nullptr), tokenHead(nullptr), tokenTail(nullptr) {}

    void addToken(string tokenData) {
        Token* newToken = new Token(tokenData);
        if (!tokenHead) {
            tokenHead = newToken;
            tokenTail = newToken;
        } else {
            newToken->prev = tokenTail;
            tokenTail->next = newToken;
            tokenTail = newToken;
        }
    }

    ~Rule() {
        Token* currentToken = tokenHead;
        while (currentToken) {
            Token* temp = currentToken;
            currentToken = currentToken->next;
            delete temp;
        }
    }
};
class Prod
{
public:
string nt;
Prod* next;
Prod* prev;

    Rule* rulehead;
    Rule* ruletail;
    Prod(string t):nt(t),next(nullptr),prev(nullptr),rulehead(nullptr),ruletail(nullptr)   { };
     void addRule(Rule* newRule) {
        if (!rulehead) {
            rulehead = ruletail = newRule;
        } else {
            newRule->prev = ruletail;
            ruletail->next = newRule;
            ruletail = newRule;
        }
    }
    ~Prod(
        
    ){
        Rule* newrule= rulehead ;
        while (newrule)
        {
            Rule* temp = newrule;
            newrule=newrule->next;
            delete temp;
        }
        
    };
};

class DoublyLinkedList
{
public:
 Prod* prodhead; 
 Prod* prodtail; 
    DoublyLinkedList():prodhead(nullptr),prodtail(nullptr) {};
    void insertattail(Prod* p){
        if (!prodhead)
        {
            prodtail=prodhead=p;
            
        }else{
            p->prev = prodtail;
            prodtail->next = p;
            prodtail = p;
        }
        

    }
    Prod* gethead(){
        return prodhead;
    }
    ~DoublyLinkedList(){
        Prod* newprod= prodhead;
        while (newprod)
        {
            Prod* temp = newprod;
            newprod= newprod->next;
            delete temp;
        }
         
    };
};



int main() {
    DoublyLinkedList prodList;
    string input;

    // Loop to allow the user to enter multiple productions
    while (true) {
        cout << "Enter a production non-terminal (or 'end' to finish): ";
        getline(cin, input);

        if (input == "end") {
            break; // Exit loop if user types 'end'
        }

        // Create a new Prod object with the non-terminal
        Prod* prod = new Prod(input);

        // Loop to allow the user to enter multiple rules for the production
        while (true) {
            cout << "Enter a rule for " << prod->nt << " (or 'done' to finish adding rules): ";
            getline(cin, input);

            if (input == "done") {
                break; // Exit loop if user types 'done'
            }

            // Create a new Rule object
            Rule* rule = new Rule();

            // Split the input string into tokens
            string token;
            for (size_t i = 0; i <= input.length(); ++i) {
                if (i == input.length() || input[i] == ' ') {
                    if (!token.empty()) {
                        rule->addToken(token); // Add each token to the rule
                        token.clear();
                    }
                } else {
                    token.push_back(input[i]);
                }
            }

            // Add the rule to the Prod manually
            if (!prod->rulehead) {
                prod->rulehead = prod->ruletail = rule;
            } else {
                rule->prev = prod->ruletail;
                prod->ruletail->next = rule;
                prod->ruletail = rule;
            }
        }

        // Insert the production into the doubly linked list
        prodList.insertattail(prod);
    }

    // Now, let's print all the productions with their rules and tokens
    Prod* currentProd = prodList.gethead();

    while (currentProd) {
        cout << currentProd->nt << " -> ";

        Rule* currentRule = currentProd->rulehead;
        while (currentRule) {
            Token* currentToken = currentRule->tokenHead;
            while (currentToken) {
                cout << currentToken->data;
                if (currentToken->next) cout << " ";
                currentToken = currentToken->next;
            }
            currentRule = currentRule->next;
            if (currentRule) cout << " | ";
        }

        cout << endl;
        currentProd = currentProd->next;
    }

    return 0;
}
