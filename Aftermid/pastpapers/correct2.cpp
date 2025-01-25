#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <unordered_map>
#include <set>
#include <sstream>
#include <iomanip>
using namespace std;

// Token structure
struct Token {
    string value;
    string type;
    Token(string v, string t) : value(v), type(t) {}
};

// Grammar symbols
namespace Grammar {
    // Non-terminals
    const string S = "S";
    const string NP = "NP";
    const string VP = "VP";
    
    // Terminals
    const string DET = "det";
    const string NOUN = "noun";
    const string VERB = "verb";
    const string END = "$";
    
    // Special symbols
    const string ERROR = "error";
    const string EPSILON = "eps";
    
    // Sets of symbols
    const set<string> nonTerminals = {S, NP, VP};
    const set<string> terminals = {DET, NOUN, VERB, END};
}

class Parser {
private:
    vector<Token> tokens;
    int currentPos;
    unordered_map<string, unordered_map<string, vector<string>>> parseTable;
    
    void initParseTable() {
        // Initialize all cells to error first
        for (const auto& nonTerm : Grammar::nonTerminals) {
            for (const auto& term : Grammar::terminals) {
                parseTable[nonTerm][term] = {Grammar::ERROR};
            }
        }
        
        // Production rules for S (Sentence)
        parseTable[Grammar::S][Grammar::DET] = {Grammar::NP, Grammar::VP};
        parseTable[Grammar::S][Grammar::NOUN] = {Grammar::NP, Grammar::VP};
        
        // Production rules for NP (Noun Phrase)
        parseTable[Grammar::NP][Grammar::DET] = {Grammar::DET, Grammar::NOUN};
        parseTable[Grammar::NP][Grammar::NOUN] = {Grammar::NOUN};
        
        // Production rules for VP (Verb Phrase)
        parseTable[Grammar::VP][Grammar::VERB] = {Grammar::VERB, Grammar::NP};
    }
    
    bool isTerminal(const string& symbol) {
        return Grammar::terminals.find(symbol) != Grammar::terminals.end();
    }
    
    bool isNonTerminal(const string& symbol) {
        return Grammar::nonTerminals.find(symbol) != Grammar::nonTerminals.end();
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
    
    void printRemainingInput() {
        cout << "Input: [ ";
        for (size_t i = currentPos; i < tokens.size(); i++) {
            cout << tokens[i].value << "(" << tokens[i].type << ") ";
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
                // Remove < and >
                token = token.substr(1, token.length()-2);
                
                // Split by comma
                size_t commaPos = token.find(',');
                if (commaPos != string::npos) {
                    string value = token.substr(0, commaPos);
                    string type = token.substr(commaPos + 1);
                    result.push_back(Token(value, type));
                }
            }
        }
        return result;
    }
    
    void printParseTable() {
        cout << "\nParsing Table:\n";
        cout << "--------------------------------------------------\n";
        cout << setw(12) << "NonTerm|";
        for (const auto& term : Grammar::terminals) {
            cout << setw(12) << term << "|";
        }
        cout << "\n--------------------------------------------------\n";
        
        for (const auto& nonTerm : Grammar::nonTerminals) {
            cout << setw(12) << nonTerm << "|";
            for (const auto& term : Grammar::terminals) {
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
    
    void printFirst() {
        cout << "\nFirst Sets:\n";
        cout << "FIRST(S) = { det, noun }\n";
        cout << "FIRST(NP) = { det, noun }\n";
        cout << "FIRST(VP) = { verb }\n";
    }
    
    void printFollow() {
        cout << "\nFollow Sets:\n";
        cout << "FOLLOW(S) = { $ }\n";
        cout << "FOLLOW(NP) = { verb, $ }\n";
        cout << "FOLLOW(VP) = { $ }\n";
    }
    
    bool parse(const string& input) {
        tokens = tokenize(input);
        tokens.push_back(Token("$", "$")); // Add end marker
        currentPos = 0;
        
        stack<string> parseStack;
        parseStack.push(Grammar::END);
        parseStack.push(Grammar::S);
        
        cout << "\nParsing Steps:\n";
        cout << "==================================================\n";
        
        while (!parseStack.empty()) {
            string top = parseStack.top();
            Token currentToken = tokens[currentPos];
            
            cout << "Current Step:\n";
            printStack(parseStack);
            printRemainingInput();
            
            parseStack.pop();
            
            if (top == Grammar::END) {
                if (currentToken.type == Grammar::END) {
                    cout << "Parsing completed successfully!\n";
                    return true;
                } else {
                    cout << "Error: Expected end of input, found " << currentToken.value << "\n";
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
                
                if (production[0] == Grammar::ERROR) {
                    cout << "Error: No production for " << top 
                         << " with input " << currentToken.type << "\n";
                    return false;
                }
                
                cout << "Applied production: " << top << " → ";
                for (const auto& symbol : production) {
                    cout << symbol << " ";
                }
                cout << "\n";
                
                // Push production in reverse order
                for (int i = production.size() - 1; i >= 0; i--) {
                    if (production[i] != Grammar::EPSILON) {
                        parseStack.push(production[i]);
                    }
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
    cout << "S → NP VP\n";
    cout << "NP → det noun | noun\n";
    cout << "VP → verb NP\n";
    
    parser.printFirst();
    parser.printFollow();
    parser.printParseTable();
    
    while (true) {
        cout << "\nEnter tokenized sentence (or 'exit' to quit)\n";
        cout << "Format: <word,type> <word,type> ...\n";
        cout << "Example: <the,det> <dog,noun> <chased,verb> <a,det> <cat,noun>\n";
        cout << "Input: ";
        
        string input;
        getline(cin, input);
        
        if (input == "exit" || input == "quit") break;
        
        bool isValid = parser.parse(input);
        cout << "\nParse Result: " << (isValid ? "Valid" : "Invalid") << " sentence\n";
    }
    
    return 0;
}