#include <iostream>
#include <map>
#include <set>
#include <string>
#include "cfg.h"

class FirstFollowCalculator {
private:
    std::map<std::string, std::set<std::string>> firstSets;
    std::map<std::string, std::set<std::string>> followSets;
    DoublyLinkedList& grammar;

    bool isTerminal(const std::string& symbol) {
        return symbol[0] != 'S' && symbol[0] != 'N' && symbol[0] != 'V' && 
               symbol[0] != 'P' && symbol[0] != 'D';
    }

    // Calculate FIRST set for a single symbol
    std::set<std::string> calculateFirstForSymbol(const std::string& symbol) {
        // If already calculated, return cached result
        if (firstSets.find(symbol) != firstSets.end()) {
            return firstSets[symbol];
        }

        std::set<std::string> firstSet;

        // If terminal, first set is just the symbol itself
        if (isTerminal(symbol)) {
            firstSet.insert(symbol);
            return firstSet;
        }

        // Find production for this non-terminal
        Prod* currentProd = grammar.gethead();
        while (currentProd && currentProd->nt != symbol) {
            currentProd = currentProd->next;
        }

        if (!currentProd) return firstSet;

        // Process each rule
        Rule* currentRule = currentProd->rulehead;
        while (currentRule) {
            Token* token = currentRule->tokenHead;
            if (!token) {  // Empty production
                firstSet.insert("ε");
            } else {
                bool allCanBeEmpty = true;
                while (token && allCanBeEmpty) {
                    std::set<std::string> tokenFirst = calculateFirstForSymbol(token->data);
                    
                    // Add all non-empty symbols
                    for (const auto& symbol : tokenFirst) {
                        if (symbol != "ε") {
                            firstSet.insert(symbol);
                        }
                    }
                    
                    // Check if this symbol can be empty
                    allCanBeEmpty = (tokenFirst.find("ε") != tokenFirst.end());
                    token = token->next;
                }
                
                // If all symbols in sequence can be empty, add ε to first set
                if (allCanBeEmpty) {
                    firstSet.insert("ε");
                }
            }
            currentRule = currentRule->next;
        }

        firstSets[symbol] = firstSet;
        return firstSet;
    }

public:
    FirstFollowCalculator(DoublyLinkedList& g) : grammar(g) {}

    void calculateFirstSets() {
        firstSets.clear();
        
        // Calculate FIRST sets for all non-terminals
        Prod* currentProd = grammar.gethead();
        while (currentProd) {
            calculateFirstForSymbol(currentProd->nt);
            currentProd = currentProd->next;
        }
    }

    void calculateFollowSets() {
        followSets.clear();
        
        // Initialize FOLLOW sets
        Prod* currentProd = grammar.gethead();
        while (currentProd) {
            followSets[currentProd->nt] = std::set<std::string>();
            currentProd = currentProd->next;
        }

        // Add $ to FOLLOW(S)
        followSets["S"].insert("$");

        // Repeat until no changes
        bool changed;
        do {
            changed = false;
            currentProd = grammar.gethead();
            
            while (currentProd) {
                Rule* currentRule = currentProd->rulehead;
                
                while (currentRule) {
                    Token* tokens[100];  // Array to store tokens
                    int tokenCount = 0;
                    
                    // Convert linked list to array for easier processing
                    Token* currentToken = currentRule->tokenHead;
                    while (currentToken) {
                        tokens[tokenCount++] = currentToken;
                        currentToken = currentToken->next;
                    }

                    // Process each token in the rule
                    for (int i = 0; i < tokenCount; i++) {
                        if (!isTerminal(tokens[i]->data)) {
                            std::set<std::string> trailer;
                            bool allCanBeEmpty = true;
                            
                            // Calculate FIRST of remaining sequence
                            for (int j = i + 1; j < tokenCount && allCanBeEmpty; j++) {
                                std::set<std::string> nextFirst = calculateFirstForSymbol(tokens[j]->data);
                                
                                // Add non-empty symbols to trailer
                                for (const auto& symbol : nextFirst) {
                                    if (symbol != "ε") {
                                        trailer.insert(symbol);
                                    }
                                }
                                
                                allCanBeEmpty = (nextFirst.find("ε") != nextFirst.end());
                            }

                            // If all remaining symbols can be empty, add FOLLOW of head
                            if (allCanBeEmpty) {
                                const auto& headFollow = followSets[currentProd->nt];
                                trailer.insert(headFollow.begin(), headFollow.end());
                            }

                            // Update FOLLOW set
                            std::set<std::string>& currentFollow = followSets[tokens[i]->data];
                            size_t originalSize = currentFollow.size();
                            currentFollow.insert(trailer.begin(), trailer.end());
                            
                            if (currentFollow.size() > originalSize) {
                                changed = true;
                            }
                        }
                    }
                    currentRule = currentRule->next;
                }
                currentProd = currentProd->next;
            }
        } while (changed);
    }

    void printSets() {
        std::cout << "\nFIRST Sets:\n";
        for (const auto& pair : firstSets) {
            std::cout << "FIRST(" << pair.first << ") = { ";
            bool first = true;
            for (const auto& symbol : pair.second) {
                if (!first) std::cout << ", ";
                std::cout << symbol;
                first = false;
            }
            std::cout << " }\n";
        }

        std::cout << "\nFOLLOW Sets:\n";
        for (const auto& pair : followSets) {
            std::cout << "FOLLOW(" << pair.first << ") = { ";
            bool first = true;
            for (const auto& symbol : pair.second) {
                if (!first) std::cout << ", ";
                std::cout << symbol;
                first = false;
            }
            std::cout << " }\n";
        }
    }
};
int main() {
    DoublyLinkedList prodList;
    
    // Example input (using shortened names):
    // S -> NP VP
    // NP -> DET N | NP PP
    // VP -> V NP | VP PP | V
    // PP -> P NP
    // DET -> 'the' | 'a'
    // N -> 'cat' | 'dog' | 'man' | 'woman'
    // V -> 'chased' | 'caught'
    // P -> 'in' | 'on' | 'with'

    // [Your existing input code here]

    // After input, calculate FIRST and FOLLOW sets
    FirstFollowCalculator calculator(prodList);
    calculator.calculateFirstSets();
    calculator.calculateFollowSets();
    calculator.printSets();

    return 0;
}