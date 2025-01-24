#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

// Function to compute FIRST of a non-terminal
void computeFirst(const string& symbol, const map<string, vector<string>>& productions, 
                 map<string, set<string>>& firstSet, set<string>& computed) {
    if (computed.find(symbol) != computed.end()) return;
    computed.insert(symbol);

    for (const string& production : productions.at(symbol)) {
        if (production == "ε") {
            firstSet[symbol].insert("ε");
            continue;
        }

        for (size_t i = 0; i < production.length(); i++) {
            string current = string(1, production[i]);
            
            if (!isupper(current[0])) {
                firstSet[symbol].insert(current);
                break;
            }

            if (firstSet.find(current) == firstSet.end()) {
                computeFirst(current, productions, firstSet, computed);
            }

            // Add everything from FIRST(current) except ε
            bool hasEpsilon = false;
            for (const string& term : firstSet[current]) {
                if (term == "ε") {
                    hasEpsilon = true;
                } else {
                    firstSet[symbol].insert(term);
                }
            }

            // If current doesn't have ε, stop here
            if (!hasEpsilon) {
                break;
            }

            // If we're at the last symbol and everything before has ε
            if (i == production.length() - 1 && hasEpsilon) {
                firstSet[symbol].insert("ε");
            }
        }
    }
}

// Function to compute FOLLOW of a non-terminal
void computeFollow(const string& symbol, const map<string, vector<string>>& productions,
                  map<string, set<string>>& firstSet, map<string, set<string>>& followSet,
                  set<string>& computed) {
    if (computed.find(symbol) != computed.end()) return;
    computed.insert(symbol);

    for (const auto& prod : productions) {
        for (const string& rule : prod.second) {
            for (size_t i = 0; i < rule.length(); i++) {
                if (string(1, rule[i]) == symbol) {
                    // If this is the last symbol
                    if (i == rule.length() - 1) {
                        if (prod.first != symbol) {
                            if (followSet.find(prod.first) == followSet.end()) {
                                set<string> temp;
                                computed.clear();
                                computeFollow(prod.first, productions, firstSet, followSet, temp);
                            }
                            followSet[symbol].insert(followSet[prod.first].begin(), followSet[prod.first].end());
                        }
                    } else {
                        // For each remaining symbol in the production
                        bool allHaveEpsilon = true;
                        size_t j = i + 1;
                        string next = string(1, rule[j]);
                            
                        if (!isupper(next[0])) {
                            followSet[symbol].insert(next);
                        } else {
                            // Add all terminals from FIRST(next)
                            for (const string& term : firstSet[next]) {
                                if (term != "ε") {
                                    followSet[symbol].insert(term);
                                }
                            }

                            // If FIRST(next) contains ε, add FOLLOW(LHS)
                            if (firstSet[next].find("ε") != firstSet[next].end() && prod.first != symbol) {
                                if (followSet.find(prod.first) == followSet.end()) {
                                    set<string> temp;
                                    computed.clear();
                                    computeFollow(prod.first, productions, firstSet, followSet, temp);
                                }
                                followSet[symbol].insert(followSet[prod.first].begin(), followSet[prod.first].end());
                            }
                        }
                    }
                }
            }
        }
    }
}

int main() {
    int n;
    cout << "Enter number of productions: ";
    cin >> n;
    cin.ignore();

    map<string, vector<string>> productions;
    cout << "Enter productions (e.g., S->AB|a):\n";
    for (int i = 0; i < n; ++i) {
        string input;
        getline(cin, input);
        stringstream ss(input);
        string lhs, rhs;
        getline(ss, lhs, '-');
        ss.ignore(1); // Skip '>'
        getline(ss, rhs);
        
        // Split RHS by '|'
        stringstream rss(rhs);
        string production;
        while (getline(rss, production, '|')) {
            productions[lhs].push_back(production);
        }
    }

    map<string, set<string>> firstSet, followSet;
    set<string> computed;

    // Compute FIRST sets
    for (const auto& prod : productions) {
        if (computed.find(prod.first) == computed.end()) {
            computeFirst(prod.first, productions, firstSet, computed);
        }
    }

    // Add $ to FOLLOW of start symbol
    string startSymbol = productions.begin()->first;
    followSet[startSymbol].insert("$");

    // Initialize FOLLOW sets for all non-terminals
    for (const auto& prod : productions) {
        followSet[prod.first];  // Create empty set for each non-terminal
    }

    // Compute FOLLOW sets (multiple passes for propagation)
    for (int pass = 0; pass < 2; pass++) {
        computed.clear();
        for (const auto& prod : productions) {
            if (computed.find(prod.first) == computed.end()) {
                computeFollow(prod.first, productions, firstSet, followSet, computed);
            }
        }
    }

    // Display FIRST sets
    cout << "\nFIRST sets:\n";
    for (const auto& set : firstSet) {
        cout << set.first << ": { ";
        for (const auto& elem : set.second) {
            cout << elem << " ";
        }
        cout << "}\n";
    }

    // Display FOLLOW sets
    cout << "\nFOLLOW sets:\n";
    for (const auto& set : followSet) {
        cout << set.first << ": { ";
        for (const auto& elem : set.second) {
            cout << elem << " ";
        }
        cout << "}\n";
    }

    return 0;
}