#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <unordered_map>
#include <set>
#include <sstream>
#include <iomanip>
using namespace std;

struct Token {
    string value;
    string type;
    Token(string v, string t) : value(v), type(t) {}
};

class Parser {
private:
    vector<Token> tokens;
    int currentPos;
    unordered_map<string, unordered_map<string, vector<string>>> parseTable;
    
    // Define all terminals and non-terminals
    const set<string> nonTerminals{"S", "NP", "VP", "PP", "DET", "N", "V", "P"};
    const set<string> terminals{"the", "a", "cat", "dog", "man", "woman", 
                               "chased", "caught", "in", "on", "with", "$"};
    
    void initParseTable() {
        // Initialize parse table with error entries
        for (const auto& nonTerm : nonTerminals) {
            for (const auto& term : terminals) {
                parseTable[nonTerm][term] = {"error"};
            }
        }
        
        // S -> NP VP
        parseTable["S"]["the"] = {"NP", "VP"};
        parseTable["S"]["a"] = {"NP", "VP"};
        parseTable["S"]["cat"] = {"NP", "VP"};
        parseTable["S"]["dog"] = {"NP", "VP"};
        parseTable["S"]["man"] = {"NP", "VP"};
        parseTable["S"]["woman"] = {"NP", "VP"};
        
        // NP -> DET N | NP PP
        parseTable["NP"]["the"] = {"DET", "N"};
        parseTable["NP"]["a"] = {"DET", "N"};
        parseTable["NP"]["cat"] = {"N"};
        parseTable["NP"]["dog"] = {"N"};
        parseTable["NP"]["man"] = {"N"};
        parseTable["NP"]["woman"] = {"N"};
        
        // VP -> V NP | VP PP | V
        parseTable["VP"]["chased"] = {"V", "NP"};
        parseTable["VP"]["caught"] = {"V", "NP"};
        
        // PP -> P NP
        parseTable["PP"]["in"] = {"P", "NP"};
        parseTable["PP"]["on"] = {"P", "NP"};
        parseTable["PP"]["with"] = {"P", "NP"};
        
        // DET -> the | a
        parseTable["DET"]["the"] = {"the"};
        parseTable["DET"]["a"] = {"a"};
        
        // N -> cat | dog | man | woman
        parseTable["N"]["cat"] = {"cat"};
        parseTable["N"]["dog"] = {"dog"};
        parseTable["N"]["man"] = {"man"};
        parseTable["N"]["woman"] = {"woman"};
        
        // V -> chased | caught
        parseTable["V"]["chased"] = {"chased"};
        parseTable["V"]["caught"] = {"caught"};
        
        // P -> in | on | with
        parseTable["P"]["in"] = {"in"};
        parseTable["P"]["on"] = {"on"};
        parseTable["P"]["with"] = {"with"};
    }
    
    bool isTerminal(const string& symbol) {
        return terminals.find(symbol) != terminals.end();
    }
    
    bool isNonTerminal(const string& symbol) {
        return nonTerminals.find(symbol) != nonTerminals.end();
    }
    
    void printStack(const stack<string>& stk) {
        stack<string> temp = stk;
        vector<string> items;
        while (!temp.empty()) {
            items.insert(items.begin(), temp.top());
            temp.pop();
        }
        cout << "Stack: [ ";
        for (const auto& item : items) {
            cout << item << " ";
        }
        cout << "]\n";
    }

public:
    Parser() : currentPos(0) {
        initParseTable();
    }
    
    vector<Token> tokenize(const string& input) {
        vector<Token> result;
        istringstream iss(input);
        string word;
        
        while (iss >> word) {
            // Check if the word is a valid terminal
            if (terminals.find(word) != terminals.end()) {
                result.push_back(Token(word, word));
            } else if (word != "$") {
                cout << "Error: Invalid token '" << word << "'\n";
                return vector<Token>();
            }
        }
        return result;
    }
    
    void printTerminals() {
        cout << "\nValid terminals in our language:\n";
        cout << "----------------------------------------\n";
        cout << "Determiners: the, a\n";
        cout << "Nouns: cat, dog, man, woman\n";
        cout << "Verbs: chased, caught\n";
        cout << "Prepositions: in, on, with\n";
        cout << "----------------------------------------\n";
    }
    
    void printParseTable() {
        cout << "\nParsing Table (showing some key entries):\n";
        cout << "--------------------------------------------------\n";
        
        // Print a subset of the parse table for readability
        vector<string> keyTerminals = {"the", "a", "cat", "chased", "in", "$"};
        
        cout << setw(12) << "NonTerm|";
        for (const auto& term : keyTerminals) {
            cout << setw(12) << term << "|";
        }
        cout << "\n--------------------------------------------------\n";
        
        for (const auto& nonTerm : nonTerminals) {
            cout << setw(12) << nonTerm << "|";
            for (const auto& term : keyTerminals) {
                string prod;
                for (const auto& sym : parseTable[nonTerm][term]) {
                    prod += sym + " ";
                }
                cout << setw(12) << (prod.empty() ? "error" : prod) << "|";
            }
            cout << "\n";
        }
        cout << "--------------------------------------------------\n";
    }
    
    bool parse(const string& input) {
        tokens = tokenize(input);
        if (tokens.empty()) {
            return false;  // Invalid tokens found
        }
        
        tokens.push_back(Token("$", "$"));
        currentPos = 0;
        
        stack<string> parseStack;
        parseStack.push("$");
        parseStack.push("S");
        
        cout << "\nParsing Steps:\n";
        cout << "==================================================\n";
        
        while (!parseStack.empty()) {
            string top = parseStack.top();
            Token currentToken = tokens[currentPos];
            
            cout << "Current token: " << currentToken.value << "\n";
            printStack(parseStack);
            
            parseStack.pop();
            
            if (top == "$") {
                if (currentToken.value == "$") {
                    cout << "Parsing completed successfully!\n";
                    return true;
                } else {
                    cout << "Error: Expected end of input\n";
                    return false;
                }
            }
            
            if (isTerminal(top)) {
                if (top == currentToken.value) {
                    cout << "Matched terminal: " << currentToken.value << "\n";
                    currentPos++;
                } else {
                    cout << "Error: Expected " << top << ", found " << currentToken.value << "\n";
                    return false;
                }
            } else if (isNonTerminal(top)) {
                vector<string> production = parseTable[top][currentToken.value];
                
                if (production[0] == "error") {
                    cout << "Error: No production for " << top 
                         << " with input " << currentToken.value << "\n";
                    return false;
                }
                
                cout << "Applied production: " << top << " → ";
                for (const auto& symbol : production) {
                    cout << symbol << " ";
                }
                cout << "\n";
                
                for (int i = production.size() - 1; i >= 0; i--) {
                    parseStack.push(production[i]);
                }
            }
            cout << "--------------------------------------------------\n";
        }
        return false;
    }
};

int main() {
    Parser parser;
    
    cout << "LL(1) Parser for English Subset\n";
    cout << "Grammar Rules:\n";
    cout << "S  → NP VP\n";
    cout << "NP → DET N | NP PP\n";
    cout << "VP → V NP | VP PP | V\n";
    cout << "PP → P NP\n";
    cout << "DET → the | a\n";
    cout << "N  → cat | dog | man | woman\n";
    cout << "V  → chased | caught\n";
    cout << "P  → in | on | with\n";
    
    parser.printTerminals();
    parser.printParseTable();
    
    while (true) {
        cout << "\nEnter sentence (or 'exit' to quit)\n";
        cout << "Example: the cat chased the dog\n";
        cout << "Input: ";
        
        string input;
        getline(cin, input);
        
        if (input == "exit" || input == "quit") break;
        
        bool isValid = parser.parse(input);
        cout << "\nParse Result: " << (isValid ? "Valid" : "Invalid") << " sentence\n";
    }
    
    return 0;
}