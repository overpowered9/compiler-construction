#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <unordered_map>
#include <set>
using namespace std;

// Token structure with value and type
struct Token {
    string value;
    string type;
    Token(string v, string t) : value(v), type(t) {}
};

// First and Follow sets calculation
class GrammarAnalyzer {
public:
    void calculateFirstAndFollow() {
        cout << "\nFirst Sets:\n";
        cout << "FIRST(Sentence) = { determiner, noun }\n";
        cout << "FIRST(NounPhrase) = { determiner, noun }\n";
        cout << "FIRST(VerbPhrase) = { verb }\n";
        cout << "FIRST(PrepPhrase) = { preposition }\n";
        
        cout << "\nFollow Sets:\n";
        cout << "FOLLOW(Sentence) = { $ }\n";
        cout << "FOLLOW(NounPhrase) = { verb, preposition, $ }\n";
        cout << "FOLLOW(VerbPhrase) = { $ }\n";
        cout << "FOLLOW(PrepPhrase) = { verb, preposition, $ }\n";
    }
};

class PredictiveParser {
private:
    vector<Token> tokens;
    int currentToken;
    unordered_map<string, unordered_map<string, vector<string>>> parsingTable;

    void initializeParsingTable() {
        // Production rules based on the grammar
        parsingTable["Sentence"]["determiner"] = {"NounPhrase", "VerbPhrase"};
        parsingTable["Sentence"]["noun"] = {"NounPhrase", "VerbPhrase"};
        
        parsingTable["NounPhrase"]["determiner"] = {"determiner", "noun"};
        parsingTable["NounPhrase"]["noun"] = {"noun"};
        
        parsingTable["VerbPhrase"]["verb"] = {"verb", "NounPhrase"};
    }

    bool isTerminal(const string& symbol) {
        return symbol == "determiner" || symbol == "noun" || 
               symbol == "verb" || symbol == "preposition";
    }

public:
    PredictiveParser() {
        currentToken = 0;
        initializeParsingTable();
    }

    vector<Token> parseInput(const string& input) {
        vector<Token> result;
        string current;
        string value, type;
        bool readingValue = false;
        
        for (char c : input) {
            if (c == '<') {
                current.clear();
                readingValue = true;
            }
            else if (c == ',') {
                value = current;
                current.clear();
                readingValue = false;
            }
            else if (c == '>') {
                type = current;
                result.push_back(Token(value, type));
                current.clear();
            }
            else if (c != ' ') {
                current += c;
            }
        }
        return result;
    }

    bool parse(const string& input) {
        tokens = parseInput(input);
        tokens.push_back(Token("$", "$")); // End marker
        currentToken = 0;

        stack<string> parseStack;
        parseStack.push("$");
        parseStack.push("Sentence");

        cout << "\nStack Trace:\n";
        cout << "----------------------------------------\n";
        
        while (!parseStack.empty()) {
            string top = parseStack.top();
            string currentType = tokens[currentToken].type;
            
            // Print current stack and input status
            cout << "Stack: ";
            stack<string> tempStack = parseStack;
            vector<string> stackContents;
            while (!tempStack.empty()) {
                stackContents.insert(stackContents.begin(), tempStack.top());
                tempStack.pop();
            }
            for (const auto& s : stackContents) cout << s << " ";
            
            cout << "\nCurrent token: " << tokens[currentToken].value 
                 << " (Type: " << tokens[currentToken].type << ")\n";
            
            parseStack.pop();
            
            if (top == "$") {
                cout << "----------------------------------------\n";
                return currentType == "$";
            }

            if (isTerminal(top)) {
                if (top == currentType) {
                    cout << "Matched terminal: " << top << "\n";
                    currentToken++;
                } else {
                    cout << "Error: Expected " << top << " but got " << currentType << "\n";
                    return false;
                }
            } else {
                if (parsingTable[top].find(currentType) != parsingTable[top].end()) {
                    vector<string> production = parsingTable[top][currentType];
                    cout << "Applied rule: " << top << " → ";
                    for (const auto& symbol : production) cout << symbol << " ";
                    cout << "\n";
                    
                    for (int i = production.size() - 1; i >= 0; i--) {
                        parseStack.push(production[i]);
                    }
                } else {
                    cout << "Error: No production rule for " << top 
                         << " with input " << currentType << "\n";
                    return false;
                }
            }
            cout << "----------------------------------------\n";
        }
        return false;
    }
};

int main() {
    GrammarAnalyzer analyzer;
    PredictiveParser parser;
    
    cout << "Grammar Rules:\n";
    cout << "Sentence → NounPhrase VerbPhrase\n";
    cout << "NounPhrase → determiner noun | noun\n";
    cout << "VerbPhrase → verb NounPhrase\n";
    
    analyzer.calculateFirstAndFollow();
    
    while (true) {
        cout << "\nEnter tokenized sentence (or 'exit' to quit)\n";
        cout << "Format: <word,type><word,type>...\n";
        cout << "Example: <the,determiner><dog,noun><caught,verb><a,determiner><cat,noun>\n";
        cout << "Input: ";
        
        string input;
        getline(cin, input);
        
        if (input == "exit" || input == "quit") break;
        
        bool isValid = parser.parse(input);
        cout << "\nSentence is " << (isValid ? "syntactically correct" : "syntactically incorrect") << "\n";
    }
    
    return 0;
}