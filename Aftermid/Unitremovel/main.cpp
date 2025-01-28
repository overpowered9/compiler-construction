#include "cfg.h"
#include "unitremoval.h"
int main() {
    DoublyLinkedList prodList;
    std::string input;

    while (true) {
        std::cout << "Enter a production non-terminal (or 'end' to finish): ";
        getline(std::cin, input);

        if (input == "end") {
            break;
        }

        Prod* prod = new Prod(input);

        while (true) {
            std::cout << "Enter a rule for " << prod->nt << " (or 'done' to finish adding rules): ";
            getline(std::cin, input);

            if (input == "done") {
                break;
            }

            Rule* rule = new Rule();
            std::string token;
            for (size_t i = 0; i <= input.length(); ++i) {
                if (i == input.length() || input[i] == ' ') {
                    if (!token.empty()) {
                        rule->addToken(token);
                        token.clear();
                    }
                } else {
                    token.push_back(input[i]);
                }
            }

            prod->addRule(rule);
        }

        prodList.insertattail(prod);
    }

    Prod* currentProd = prodList.gethead();
    while (currentProd) {
        std::cout << currentProd->nt << " -> ";

        Rule* currentRule = currentProd->rulehead;
        while (currentRule) {
            Token* currentToken = currentRule->tokenHead;
            while (currentToken) {
                std::cout << currentToken->data;
                if (currentToken->next) std::cout << " ";
                currentToken = currentToken->next;
            }
            currentRule = currentRule->next;
            if (currentRule) std::cout << " | ";
        }

        std::cout << std::endl;
        currentProd = currentProd->next;
    }
    DoublyLinkedList* newProdList = removeNullProduction(prodList);
    
    std::cout << "\nModified CFG (Null Productions Removed):\n";
    Prod* currentProd2 = newProdList->gethead();
    while (currentProd2) {
        std::cout << currentProd2->nt << " -> ";
        Rule* currentRule = currentProd2->rulehead;
        while (currentRule) {
            Token* currentToken = currentRule->tokenHead;
            while (currentToken) {
                std::cout << currentToken->data;
                if (currentToken->next) std::cout << " ";
                currentToken = currentToken->next;
            }
            currentRule = currentRule->next;
            if (currentRule) std::cout << " | ";
        }
        std::cout << std::endl;
        currentProd2 = currentProd2->next;
    }
DoublyLinkedList* UnitProdList = removeUnitProduction(prodList);
 std::cout << "\nModified CFG (unit Productions Removed):\n";
    Prod* currentProd3 = UnitProdList->gethead();
    while (currentProd3) {
        std::cout << currentProd3->nt << " -> ";
        Rule* currentRule = currentProd3->rulehead;
        while (currentRule) {
            Token* currentToken = currentRule->tokenHead;
            while (currentToken) {
                std::cout << currentToken->data;
                if (currentToken->next) std::cout << " ";
                currentToken = currentToken->next;
            }
            currentRule = currentRule->next;
            if (currentRule) std::cout << " | ";
        }
        std::cout << std::endl;
        currentProd3 = currentProd3->next;
    }
    return 0;
}