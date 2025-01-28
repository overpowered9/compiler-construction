#include <unordered_map>
#include <unordered_set>
#include <string>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include "cfg.h"

#include <vector>

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <iomanip>
class FirstFollowCalculator {
private:
    std::unordered_map<std::string, std::unordered_set<std::string>> firstSets;
    std::unordered_map<std::string, std::unordered_set<std::string>> followSets;
    DoublyLinkedList& grammar;

    bool isTerminal(const std::string& token) {
        return token[0] == '\'' || (token == "the" || token == "a");
    }

    std::string stripQuotes(const std::string& token) {
        if (token.size() >= 2 && token[0] == '\'' && token.back() == '\'') {
            return token.substr(1, token.size() - 2);
        }
        return token;
    }

    Prod* findProduction(const std::string& nt) {
        Prod* current = grammar.gethead();
        while (current) {
            if (current->nt == nt) return current;
            current = current->next;
        }
        return nullptr;
    }

    void computeFirstSet(const std::string& symbol) {
        if (firstSets.find(symbol) != firstSets.end()) return;

        firstSets[symbol] = std::unordered_set<std::string>();

        if (isTerminal(symbol)) {
            firstSets[symbol].insert(stripQuotes(symbol));
            return;
        }

        Prod* prod = findProduction(symbol);
        if (!prod) return;

        Rule* currentRule = prod->rulehead;
        while (currentRule) {
            Token* firstToken = currentRule->tokenHead;
            if (!firstToken) {
                currentRule = currentRule->next;
                continue;
            }

            computeFirstSet(firstToken->data);
            auto& firstSymbolSet = firstSets[firstToken->data];
            firstSets[symbol].insert(firstSymbolSet.begin(), firstSymbolSet.end());
            
            currentRule = currentRule->next;
        }
    }

    void computeFollowSet(const std::string& symbol) {
        if (followSets.find(symbol) != followSets.end()) return;

        followSets[symbol] = std::unordered_set<std::string>();
        
        // Add $ to FOLLOW(S) where S is the start symbol
        if (symbol == grammar.gethead()->nt) {
            followSets[symbol].insert("$");
        }

        // For each production
        Prod* currentProd = grammar.gethead();
        while (currentProd) {
            Rule* currentRule = currentProd->rulehead;
            
            // For each rule in the production
            while (currentRule) {
                Token* currentToken = currentRule->tokenHead;
                
                // For each token in the rule
                while (currentToken) {
                    // If we find our symbol
                    if (!isTerminal(currentToken->data) && currentToken->data == symbol) {
                        // Get the next token
                        Token* nextToken = currentToken->next;
                        
                        if (nextToken) {
                            // Add FIRST(next) to FOLLOW(symbol)
                            computeFirstSet(nextToken->data);
                            auto& firstNext = firstSets[nextToken->data];
                            followSets[symbol].insert(firstNext.begin(), firstNext.end());
                        } else {
                            // If there's no next token, add FOLLOW(lefthand) to FOLLOW(symbol)
                            // But only if lefthand != symbol (to prevent infinite recursion)
                            if (currentProd->nt != symbol) {
                                computeFollowSet(currentProd->nt);
                                auto& followLeft = followSets[currentProd->nt];
                                followSets[symbol].insert(followLeft.begin(), followLeft.end());
                            }
                        }
                    }
                    currentToken = currentToken->next;
                }
                currentRule = currentRule->next;
            }
            currentProd = currentProd->next;
        }
    }

public:
    FirstFollowCalculator(DoublyLinkedList& g) : grammar(g) {}

    void computeAllSets() {
        // First compute all FIRST sets
        Prod* current = grammar.gethead();
        while (current) {
            computeFirstSet(current->nt);
            current = current->next;
        }

        // Then compute all FOLLOW sets
        current = grammar.gethead();
        while (current) {
            computeFollowSet(current->nt);
            current = current->next;
        }
    }

    void printSets() {
        std::cout << "\nFIRST Sets:\n";
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

        std::cout << "\nFOLLOW Sets:\n";
        current = grammar.gethead();
        while (current) {
            std::cout << "FOLLOW(" << current->nt << ") = { ";
            bool first = true;
            for (const auto& terminal : followSets[current->nt]) {
                if (!first) std::cout << ", ";
                std::cout << terminal;
                first = false;
            }
            std::cout << " }\n";
            current = current->next;
        }
    }

    // Getters for use in predictive parsing
    const std::unordered_map<std::string, std::unordered_set<std::string>>& getFirstSets() const {
        return firstSets;
    }

    const std::unordered_map<std::string, std::unordered_set<std::string>>& getFollowSets() const {
        return followSets;
    }
};
class PredictiveParser {
private:
    std::map<std::pair<std::string, std::string>, std::vector<std::string>> parsingTable;
    const std::unordered_map<std::string, std::unordered_set<std::string>>& firstSets;
    const std::unordered_map<std::string, std::unordered_set<std::string>>& followSets;
    DoublyLinkedList& grammar;

    bool isTerminal(const std::string& symbol) {
        return symbol[0] == '\'' || (symbol == "the" || symbol == "a");
    }

    std::string stripQuotes(const std::string& token) {
        if (token.size() >= 2 && token[0] == '\'' && token.back() == '\'') {
            return token.substr(1, token.size() - 2);
        }
        return token;
    }

    void constructParsingTable() {
        Prod* currentProd = grammar.gethead();
        while (currentProd) {
            std::string nonTerminal = currentProd->nt;
            Rule* currentRule = currentProd->rulehead;
            
            while (currentRule) {
                std::vector<std::string> production;
                Token* token = currentRule->tokenHead;
                while (token) {
                    production.push_back(token->data);
                    token = token->next;
                }

                Token* firstToken = currentRule->tokenHead;
                if (firstToken) {
                    std::string firstSymbol = firstToken->data;
                    if (isTerminal(firstSymbol)) {
                        // Strip quotes for terminal symbols in the parsing table
                        parsingTable[{nonTerminal, stripQuotes(firstSymbol)}] = production;
                    } else {
                        for (const auto& terminal : firstSets.at(firstSymbol)) {
                            parsingTable[{nonTerminal, terminal}] = production;
                        }
                    }
                }
                currentRule = currentRule->next;
            }
            currentProd = currentProd->next;
        }
    }

public:
    PredictiveParser(DoublyLinkedList& g,
                    const std::unordered_map<std::string, std::unordered_set<std::string>>& first,
                    const std::unordered_map<std::string, std::unordered_set<std::string>>& follow)
        : grammar(g), firstSets(first), followSets(follow) {
        constructParsingTable();
    }

    bool parse(const std::vector<std::string>& input) {
        std::vector<std::string> stack;
        stack.push_back("$");
        stack.push_back(grammar.gethead()->nt);
        size_t inputPos = 0;

        while (!stack.empty() && inputPos <= input.size()) {
            std::string top = stack.back();
            std::string currentInput = inputPos < input.size() ? input[inputPos] : "$";
            
            std::cout << "Stack top: " << top << ", Input: " << currentInput << "\n"; // Debug print
            
            if (top == "$" && currentInput == "$") {
                return true;
            }
            
            stack.pop_back();
            
            if (isTerminal(top)) {
                if (stripQuotes(top) == currentInput) {
                    inputPos++;
                } else {
                    return false;
                }
            } else if (top == "$") {
                if (currentInput == "$") {
                    continue;
                } else {
                    return false;
                }
            } else {
                auto entry = parsingTable.find({top, currentInput});
                if (entry == parsingTable.end()) {
                    return false;
                }
                
                const auto& production = entry->second;
                for (auto it = production.rbegin(); it != production.rend(); ++it) {
                    stack.push_back(*it);
                }
            }
        }
        return inputPos >= input.size();
    }

    // Debug method to print parsing table
    void printTable() {
        std::cout << "Parsing Table:\n";
        for (const auto& entry : parsingTable) {
            std::cout << entry.first.first << ", " << entry.first.second << " -> ";
            for (const auto& symbol : entry.second) {
                std::cout << symbol << " ";
            }
            std::cout << "\n";
        }
    }
};



int main() {
    DoublyLinkedList prodList;

    // E -> T + E | T
    Prod* prodE = new Prod("E");
    Rule* ruleE1 = new Rule();
    ruleE1->addToken("T");
    ruleE1->addToken("'+'");
    ruleE1->addToken("E");
    Rule* ruleE2 = new Rule();
    ruleE2->addToken("T");
    prodE->addRule(ruleE1);
    prodE->addRule(ruleE2);
    prodList.insertattail(prodE);

    // T -> num
    Prod* prodT = new Prod("T");
    Rule* ruleT = new Rule();
    ruleT->addToken("'num'");
    prodT->addRule(ruleT);
    prodList.insertattail(prodT);

    // Print the grammar
    std::cout << "Grammar:\n";
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

    FirstFollowCalculator calculator(prodList);
    calculator.computeAllSets();

    PredictiveParser parser(prodList, calculator.getFirstSets(), calculator.getFollowSets());
    
    // Print parsing table for debugging
    parser.printTable();

    // Test some inputs
    std::vector<std::string> input1 = {"num", "+", "num"};
    std::cout << "\nTesting: num + num\n";
    std::cout << "Result: " << (parser.parse(input1) ? "valid" : "invalid") << "\n";

    std::vector<std::string> input2 = {"num"};
    std::cout << "\nTesting: num\n";
    std::cout << "Result: " << (parser.parse(input2) ? "valid" : "invalid") << "\n";

    return 0;
}