#include <iostream>
#include <map>
#include <set>
#include <string>
#include "cfg.h"

#include <vector>

#include <unordered_map>
#include <unordered_set>
#include <string>

bool isTerminal(const std::string& token) {
    return token[0] == '\'' || 
           (token == "the" || token == "a");
}

std::string stripQuotes(const std::string& token) {
    if (token.size() >= 2 && token[0] == '\'' && token.back() == '\'') {
        return token.substr(1, token.size() - 2);
    }
    return token;
}

class FirstSetCalculator {
private:
    std::unordered_map<std::string, std::unordered_set<std::string>> firstSets;
     DoublyLinkedList& grammar;

    Prod* findProduction(const std::string& nt) {
        Prod* current = grammar.gethead();
        while (current) {
            if (current->nt == nt) return current;
            current = current->next;
        }
        return nullptr;
    }

    void computeFirstSet(const std::string& symbol) {
        // If already computed, return
        if (firstSets.find(symbol) != firstSets.end()) return;

        firstSets[symbol] = std::unordered_set<std::string>();

        // If terminal, FIRST(symbol) = {symbol}
        if (isTerminal(symbol)) {
            firstSets[symbol].insert(stripQuotes(symbol));
            return;
        }

        // Find the production for this non-terminal
        Prod* prod = findProduction(symbol);
        if (!prod) return;

        // For each rule of this production
        Rule* currentRule = prod->rulehead;
        while (currentRule) {
            Token* firstToken = currentRule->tokenHead;
            if (!firstToken) {
                currentRule = currentRule->next;
                continue;
            }

            // Compute FIRST set of the first symbol in the rule
            computeFirstSet(firstToken->data);
            
            // Add all elements from FIRST(firstSymbol) to FIRST(symbol)
            auto& firstSymbolSet = firstSets[firstToken->data];
            firstSets[symbol].insert(firstSymbolSet.begin(), firstSymbolSet.end());
            
            currentRule = currentRule->next;
        }
    }

public:
    FirstSetCalculator(DoublyLinkedList& g) : grammar(g) {}

    std::unordered_map<std::string, std::unordered_set<std::string>> computeAllFirstSets() {
        Prod* current = grammar.gethead();
        while (current) {
            computeFirstSet(current->nt);
            current = current->next;
        }
        return firstSets;
    }

    void printFirstSets() {
        std::cout << "\nFIRST Sets:\n";
        // Only print FIRST sets for non-terminals
        Prod* current = grammar.gethead();
        while (current) {
            std::cout << "FIRST(" << current->nt << ") = { ";
            bool first = true;
            for (const auto& terminal : firstSets[current->nt]) {
                if (!first) std::cout << ", ";
                std::cout << terminal;
                first = false;
            }
            std::cout << " }\n";
            current = current->next;
        }
    }
};
// int main() {
//     DoublyLinkedList prodList;
//     std::string input;

//     while (true) {
//         std::cout << "Enter a production non-terminal (or 'end' to finish): ";
//         getline(std::cin, input);

//         if (input == "end") {
//             break;
//         }

//         Prod* prod = new Prod(input);

//         while (true) {
//             std::cout << "Enter a rule for " << prod->nt << " (or 'done' to finish adding rules): ";
//             getline(std::cin, input);

//             if (input == "done") {
//                 break;
//             }

//             Rule* rule = new Rule();
//             std::string token;
//             for (size_t i = 0; i <= input.length(); ++i) {
//                 if (i == input.length() || input[i] == ' ') {
//                     if (!token.empty()) {
//                         rule->addToken(token);
//                         token.clear();
//                     }
//                 } else {
//                     token.push_back(input[i]);
//                 }
//             }

//             prod->addRule(rule);
//         }

//         prodList.insertattail(prod);
//     }

//     Prod* currentProd = prodList.gethead();
//     while (currentProd) {
//         std::cout << currentProd->nt << " -> ";

//         Rule* currentRule = currentProd->rulehead;
//         while (currentRule) {
//             Token* currentToken = currentRule->tokenHead;
//             while (currentToken) {
//                 std::cout << currentToken->data;
//                 if (currentToken->next) std::cout << " ";
//                 currentToken = currentToken->next;
//             }
//             currentRule = currentRule->next;
//             if (currentRule) std::cout << " | ";
//         }

//         std::cout << std::endl;
//         currentProd = currentProd->next;
//     }
//     // Example input (using shortened names):
//     // S -> NP VP
//     // NP -> DET N | NP PP
//     // VP -> V NP | VP PP | V
//     // PP -> P NP
//     // DET -> 'the' | 'a'
//     // N -> 'cat' | 'dog' | 'man' | 'woman'
//     // V -> 'chased' | 'caught'
//     // P -> 'in' | 'on' | 'with'

//     // [Your existing input code here]

//     // After input, calculate FIRST and FOLLOW sets
//     FirstFollowCalculator calculator(prodList);
//     calculator.calculateFirstSets();
//     calculator.calculateFollowSets();
//     calculator.printSets();

//     return 0;
// }


int main() {
    DoublyLinkedList prodList;

    // S -> NP VP
    Prod* prodS = new Prod("S");
    Rule* ruleS = new Rule();
    ruleS->addToken("NP");
    ruleS->addToken("VP");
    prodS->addRule(ruleS);
    prodList.insertattail(prodS);

    // NP -> DET N | NP PP
    Prod* prodNP = new Prod("NP");
    Rule* ruleNP1 = new Rule();
    ruleNP1->addToken("DET");
    ruleNP1->addToken("N");
    Rule* ruleNP2 = new Rule();
    ruleNP2->addToken("NP");
    ruleNP2->addToken("PP");
    prodNP->addRule(ruleNP1);
    prodNP->addRule(ruleNP2);
    prodList.insertattail(prodNP);

    // VP -> V NP | VP PP | V
    Prod* prodVP = new Prod("VP");
    Rule* ruleVP1 = new Rule();
    ruleVP1->addToken("V");
    ruleVP1->addToken("NP");
    Rule* ruleVP2 = new Rule();
    ruleVP2->addToken("VP");
    ruleVP2->addToken("PP");
    Rule* ruleVP3 = new Rule();
    ruleVP3->addToken("V");
    prodVP->addRule(ruleVP1);
    prodVP->addRule(ruleVP2);
    prodVP->addRule(ruleVP3);
    prodList.insertattail(prodVP);

    // PP -> P NP
    Prod* prodPP = new Prod("PP");
    Rule* rulePP = new Rule();
    rulePP->addToken("P");
    rulePP->addToken("NP");
    prodPP->addRule(rulePP);
    prodList.insertattail(prodPP);

    // DET -> 'the' | 'a'
    Prod* prodDET = new Prod("DET");
    Rule* ruleDET1 = new Rule();
    ruleDET1->addToken("the");
    Rule* ruleDET2 = new Rule();
    ruleDET2->addToken("a");
    prodDET->addRule(ruleDET1);
    prodDET->addRule(ruleDET2);
    prodList.insertattail(prodDET);

    // N -> 'cat' | 'dog' | 'man' | 'woman'
    Prod* prodN = new Prod("N");
    Rule* ruleN1 = new Rule();
    ruleN1->addToken("'cat'");
    Rule* ruleN2 = new Rule();
    ruleN2->addToken("'dog'");
    Rule* ruleN3 = new Rule();
    ruleN3->addToken("'man'");
    Rule* ruleN4 = new Rule();
    ruleN4->addToken("'woman'");
    prodN->addRule(ruleN1);
    prodN->addRule(ruleN2);
    prodN->addRule(ruleN3);
    prodN->addRule(ruleN4);
    prodList.insertattail(prodN);

    // V -> 'chased' | 'caught'
    Prod* prodV = new Prod("V");
    Rule* ruleV1 = new Rule();
    ruleV1->addToken("'chased'");
    Rule* ruleV2 = new Rule();
    ruleV2->addToken("'caught'");
    prodV->addRule(ruleV1);
    prodV->addRule(ruleV2);
    prodList.insertattail(prodV);

    // P -> 'in' | 'on' | 'with'
    Prod* prodP = new Prod("P");
    Rule* ruleP1 = new Rule();
    ruleP1->addToken("'in'");
    Rule* ruleP2 = new Rule();
    ruleP2->addToken("'on'");
    Rule* ruleP3 = new Rule();
    ruleP3->addToken("'with'");
    prodP->addRule(ruleP1);
    prodP->addRule(ruleP2);
    prodP->addRule(ruleP3);
    prodList.insertattail(prodP);

    // Print the original grammar
    std::cout << "Original Grammar:\n";
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

    FirstSetCalculator calculator(prodList);
    auto firstSets = calculator.computeAllFirstSets();
    calculator.printFirstSets();
    return 0;
}