#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <sstream> // For stringstream
using namespace std;

// Token types for lexical analysis
enum class TokenType {
    DETERMINER,  // the, a
    NOUN,        // cat, dog, man, woman
    VERB,        // chased, caught
    PREPOSITION, // in, on, with
    END         // $ (end marker)
};

// Token structure with type and value
struct Token {
    TokenType type;
    string value;
    Token(TokenType t, string v) : type(t), value(v) {}
};

// Parse tree node structure
struct Node {
    string symbol;      // Grammar symbol (terminal or non-terminal)
    string value;       // Actual token value (for terminals)
    vector<Node*> children;
    
    Node(string s) : symbol(s) {}
    ~Node() {
        for(auto child : children) {
            delete child;
        }
    }
};

// Main parser class
class Parser {
private:
    // Tokens and current position
    vector<Token> tokens;
    int currentToken;
    
    // Valid token sets
    unordered_set<string> determiners {"the", "a"};
    unordered_set<string> nouns {"cat", "dog", "man", "woman"};
    unordered_set<string> verbs {"chased", "caught"};
    unordered_set<string> prepositions {"in", "on", "with"};
    
    // Parsing table (LL(1) grammar)
    unordered_map<string, unordered_map<TokenType, vector<string>>> parsingTable;
    
    // Convert token type to string for debugging
    string getTokenTypeName(TokenType type) {
        switch(type) {
            case TokenType::DETERMINER: return "DETERMINER";
            case TokenType::NOUN: return "NOUN";
            case TokenType::VERB: return "VERB";
            case TokenType::PREPOSITION: return "PREPOSITION";
            case TokenType::END: return "END";
            default: return "UNKNOWN";
        }
    }
    
    // Initialize the parsing table with grammar rules
    void initializeParsingTable() {
        // Basic sentence structure
        parsingTable["Sentence"][TokenType::DETERMINER] = {"NounPhrase", "VerbPhrase"};
        parsingTable["Sentence"][TokenType::NOUN] = {"NounPhrase", "VerbPhrase"};
        
        // Noun phrase structure
        parsingTable["NounPhrase"][TokenType::DETERMINER] = {"Determiner", "Noun"};
        parsingTable["NounPhrase"][TokenType::NOUN] = {"Noun"};
        
        // Verb phrase structure
        parsingTable["VerbPhrase"][TokenType::VERB] = {"Verb", "NounPhrase"};
        
        // Optional prepositional phrase (can be added later if needed)
        parsingTable["PrepositionalPhrase"][TokenType::PREPOSITION] = {"Preposition", "NounPhrase"};
    }
    
    // Check if a symbol is a terminal
    bool isTerminal(const string& symbol) {
        return symbol == "Determiner" || symbol == "Noun" || 
               symbol == "Verb" || symbol == "Preposition";
    }
    
    // Get token type from string
    TokenType getTokenType(const string& token) {
        if (determiners.find(token) != determiners.end()) return TokenType::DETERMINER;
        if (nouns.find(token) != nouns.end()) return TokenType::NOUN;
        if (verbs.find(token) != verbs.end()) return TokenType::VERB;
        if (prepositions.find(token) != prepositions.end()) return TokenType::PREPOSITION;
        throw runtime_error("Invalid token: " + token);
    }
    
    // Check if terminal matches current token
    bool matchTerminal(const string& terminal, const Token& token) {
        if (terminal == "Determiner") return token.type == TokenType::DETERMINER;
        if (terminal == "Noun") return token.type == TokenType::NOUN;
        if (terminal == "Verb") return token.type == TokenType::VERB;
        if (terminal == "Preposition") return token.type == TokenType::PREPOSITION;
        return false;
    }

public:
    Parser() {
        currentToken = 0;
        initializeParsingTable();
        cout << "Parser initialized successfully\n";
    }

    // Main parsing function
    Node* parse(const vector<string>& input) {
        cout << "\nParsing sentence: ";
        for (const auto& word : input) cout << word << " ";
        cout << endl;

        // Convert input to tokens
        tokens.clear();
        for (const auto& word : input) {
            tokens.push_back(Token(getTokenType(word), word));
        }
        tokens.push_back(Token(TokenType::END, "$"));
        currentToken = 0;

        // Initialize stacks
        stack<string> parseStack;
        parseStack.push("$");
        parseStack.push("Sentence");

        Node* root = new Node("Sentence");
        stack<Node*> nodeStack;
        nodeStack.push(root);

        // Main parsing loop
        while (!parseStack.empty()) {
            string top = parseStack.top();
            parseStack.pop();
            Node* currentNode = nodeStack.top();
            nodeStack.pop();

            cout << "Processing: " << top << " | Current token: " << tokens[currentToken].value << endl;

            // Check for end of input
            if (top == "$") {
                if (tokens[currentToken].type == TokenType::END) {
                    cout << "Parsing completed successfully!\n";
                    return root;
                }
                throw runtime_error("Unexpected token at end");
            }

            // Handle terminals
            if (isTerminal(top)) {
                if (matchTerminal(top, tokens[currentToken])) {
                    currentNode->value = tokens[currentToken].value;
                    currentToken++;
                } else {
                    throw runtime_error("Expected " + top + " but got " + tokens[currentToken].value);
                }
                continue;
            }

            // Handle non-terminals
            auto productions = parsingTable[top].find(tokens[currentToken].type);
            if (productions != parsingTable[top].end()) {
                for (int i = productions->second.size() - 1; i >= 0; i--) {
                    string symbol = productions->second[i];
                    parseStack.push(symbol);
                    Node* newNode = new Node(symbol);
                    currentNode->children.insert(currentNode->children.begin(), newNode);
                    nodeStack.push(newNode);
                }
            } else {
                throw runtime_error("No production rule for " + top + " with token " + tokens[currentToken].value);
            }
        }

        throw runtime_error("Unexpected end of input");
    }

    // Print parse tree
    void printTree(Node* node, int depth = 0) {
        string indent(depth * 2, ' ');
        cout << indent << node->symbol;
        if (!node->value.empty()) cout << " (" << node->value << ")";
        cout << endl;
        
        for (auto child : node->children) {
            printTree(child, depth + 1);
        }
    }
};
/////////////////////////
vector<string> splitString(const string& str) {
    vector<string> tokens;
    string token;
    for (char c : str) {
        if (c == ' ' || c == '\t') {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
        } else {
            token += tolower(c);  // Convert to lowercase for consistency
        }
    }
    if (!token.empty()) {
        tokens.push_back(token);
    }
    return tokens;
}
// Main function with test cases
int main() {
    Parser parser;
    string input;
    // vector<vector<string>> testCases = {
    //     {"the", "cat", "caught", "the", "dog"},
    //     {"a", "dog", "chased", "the", "cat"},
    //     {"the", "cat", "caught", "the", "cat"},
    // };
    vector<vector<string>> testCases;

    cout << "Enter a sentence: ";

    getline(cin, input);
    vector<string> newTestCase;
    stringstream ss(input);
     string word;
    while (ss >> word) {
        newTestCase.push_back(word);
    }
    // // Adding the new test case to the testCases vector
    // testCases.push_back(newTestCase);
    // cout << "Updated Test Cases:" << endl;
    // for (const auto& testCase : testCases) {
    //     for (const auto& word : testCase) {
    //         cout << word << " ";
    //     }
    //     cout << endl;
    // }
    for (const auto& sentence : testCases) {
        try {
            Node* parseTree = parser.parse(sentence);
            cout << "\nParse Tree:\n";
            cout << "====================\n";
            parser.printTree(parseTree);
            cout << "====================\n";
            delete parseTree;
        } catch (const runtime_error& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
    
    return 0;
}