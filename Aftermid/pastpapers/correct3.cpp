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
    
    // Define valid terminals for each type
    unordered_map<string, set<string>> validTerminals = {
        {"det", {"a","the"}},
        {"noun", {"cat", "dog"}},
        {"verb", {"chased", "caught"}}
    };

    const set<string> nonTerminals{"S", "NP", "VP"};
    const set<string> terminalTypes{"det", "noun", "verb", "$"};
    
    void initParseTable() {
        // Initialize error entries
        for (const auto& nonTerm : nonTerminals) {
            for (const auto& term : terminalTypes) {
                parseTable[nonTerm][term] = {"error"};
            }
        }
        
        // Production rules
        parseTable["S"]["det"] = {"NP", "VP"};
        parseTable["S"]["noun"] = {"NP", "VP"};
        
        parseTable["NP"]["det"] = {"det", "noun"};
        parseTable["NP"]["noun"] = {"noun"};
        
        parseTable["VP"]["verb"] = {"verb", "NP"};
    }
    
    bool isTerminal(const string& symbol) {
        return terminalTypes.find(symbol) != terminalTypes.end();
    }
    
    bool isNonTerminal(const string& symbol) {
        return nonTerminals.find(symbol) != nonTerminals.end();
    }

    bool isValidTerminal(const string& value, const string& type) {
        if (type == "$") return value == "$";
        auto it = validTerminals.find(type);
        if (it == validTerminals.end()) return false;
        return it->second.find(value) != it->second.end();
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
        string token;
        
        while (iss >> token) {
            if (token[0] == '<' && token[token.length()-1] == '>') {
                token = token.substr(1, token.length()-2);
                size_t commaPos = token.find(',');
                if (commaPos != string::npos) {
                    string value = token.substr(0, commaPos);
                    string type = token.substr(commaPos + 1);
                    
                    // Validate the token
                    if (!isValidTerminal(value, type)) {
                        cout << "Error: Invalid terminal '" << value 
                             << "' for type '" << type << "'\n";
                        return vector<Token>();
                    }
                    
                    result.push_back(Token(value, type));
                }
            }
        }
        return result;
    }
    
    void printValidTerminals() {
        cout << "\nValid terminals in our language:\n";
        cout << "----------------------------------------\n";
        for (const auto& category : validTerminals) {
            cout << category.first << ": { ";
            for (const auto& term : category.second) {
                cout << term << " ";
            }
            cout << "}\n";
        }
        cout << "----------------------------------------\n";
    }
    
    void printParseTable() {
        cout << "\nParsing Table:\n";
        cout << "--------------------------------------------------\n";
        cout << setw(12) << "NonTerm|";
        for (const auto& term : terminalTypes) {
            cout << setw(12) << term << "|";
        }
        cout << "\n--------------------------------------------------\n";
        
        for (const auto& nonTerm : nonTerminals) {
            cout << setw(12) << nonTerm << "|";
            for (const auto& term : terminalTypes) {
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
            
            cout << "Current token: " << currentToken.value 
                 << " (" << currentToken.type << ")\n";
            printStack(parseStack);
            
            parseStack.pop();
            
            if (top == "$") {
                if (currentToken.type == "$") {
                    cout << "Parsing completed successfully!\n";
                    return true;
                } else {
                    cout << "Error: Expected end of input\n";
                    return false;
                }
            }
            
            if (isTerminal(top)) {
                if (top == currentToken.type) {
                    cout << "Matched terminal: " << currentToken.value << "\n";
                    currentPos++;
                } else {
                    cout << "Error: Expected " << top << ", found " << currentToken.type << "\n";
                    return false;
                }
            } else if (isNonTerminal(top)) {
                vector<string> production = parseTable[top][currentToken.type];
                
                if (production[0] == "error") {
                    cout << "Error: No production for " << top 
                         << " with input " << currentToken.type << "\n";
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
    
    cout << "LL(1) Parser with Terminal Validation\n";
    cout << "Grammar Rules:\n";
    cout << "S → NP VP\n";
    cout << "NP → det noun | noun\n";
    cout << "VP → verb NP\n";
    
    parser.printValidTerminals();
    parser.printParseTable();
    
    while (true) {
        cout << "\nEnter tokenized sentence (or 'exit' to quit)\n";
        cout << "Format: <word,type> <word,type> ...\n";
        cout << "Example: <the,det> <cat,noun> <caught,verb> <the,det> <mouse,noun>\n";
        cout << "Input: ";
        
        string input;
        getline(cin, input);
        
        if (input == "exit" || input == "quit") break;
        
        bool isValid = parser.parse(input);
        cout << "\nParse Result: " << (isValid ? "Valid" : "Invalid") << " sentence\n";
    }
    
    return 0;
}