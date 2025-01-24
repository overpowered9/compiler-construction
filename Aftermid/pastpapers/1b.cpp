#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
using namespace std;

struct Token {
    string type;
    string value;
    Token(string t, string v) : type(t), value(v) {}
};

class Parser {
private:
    vector<Token> tokens;
    size_t currentToken;
    int indentLevel;
    
    string getIndent() {
        return string(indentLevel * 2, ' ');
    }
    
    void debugPrint(const string& message) {
        cout << getIndent() << message << endl;
    }

    void debugTokenInfo() {
        if (currentToken < tokens.size()) {
            debugPrint("Current token: " + tokens[currentToken].type + " ('" + 
                      tokens[currentToken].value + "')");
        } else {
            debugPrint("No more tokens to process");
        }
    }
    
    bool match(const string& expected_type) {
        debugPrint("Matching '" + expected_type + "'");
        debugTokenInfo();
        
        if (currentToken >= tokens.size()) {
            debugPrint("→ Match failed: End of tokens");
            return false;
        }
        
        bool matches = tokens[currentToken].type == expected_type;
        debugPrint("→ Match " + string(matches ? "successful" : "failed"));
        return matches;
    }
    
    void consume() {
        if (currentToken < tokens.size()) {
            debugPrint("Consuming token: " + tokens[currentToken].type + 
                      " ('" + tokens[currentToken].value + "')");
            currentToken++;
        }
    }
    
    // Grammar Rules Implementation
    bool sentence() {
        debugPrint("\n=== Attempting to parse SENTENCE ===");
        indentLevel++;
        
        debugPrint("Checking for NounPhrase followed by VerbPhrase");
        bool hasNounPhrase = nounPhrase();
        if (!hasNounPhrase) {
            debugPrint("Failed to parse NounPhrase");
            indentLevel--;
            return false;
        }
        
        bool hasVerbPhrase = verbPhrase();
        if (!hasVerbPhrase) {
            debugPrint("Failed to parse VerbPhrase");
            indentLevel--;
            return false;
        }
        
        debugPrint("Successfully parsed complete sentence");
        indentLevel--;
        return true;
    }
    
    bool nounPhrase() {
        debugPrint("\n--- Attempting to parse NOUN PHRASE ---");
        indentLevel++;
        
        debugPrint("Checking for Determiner followed by Noun");
        bool hasDeterminer = determiner();
        if (!hasDeterminer) {
            debugPrint("Failed to parse Determiner");
            indentLevel--;
            return false;
        }
        
        bool hasNoun = noun();
        if (!hasNoun) {
            debugPrint("Failed to parse Noun");
            indentLevel--;
            return false;
        }
        
        debugPrint("Successfully parsed noun phrase");
        indentLevel--;
        return true;
    }
    
    bool verbPhrase() {
        debugPrint("\n--- Attempting to parse VERB PHRASE ---");
        indentLevel++;
        
        debugPrint("Checking for Verb followed by NounPhrase");
        bool hasVerb = verb();
        if (!hasVerb) {
            debugPrint("Failed to parse Verb");
            indentLevel--;
            return false;
        }
        
        bool hasNounPhrase = nounPhrase();
        if (!hasNounPhrase) {
            debugPrint("Failed to parse NounPhrase in VerbPhrase");
            indentLevel--;
            return false;
        }
        
        debugPrint("Successfully parsed verb phrase");
        indentLevel--;
        return true;
    }
    
    bool determiner() {
        debugPrint("Checking for DETERMINER");
        indentLevel++;
        bool result = match("determiner");
        if (result) {
            consume();
        }
        indentLevel--;
        return result;
    }
    
    bool noun() {
        debugPrint("Checking for NOUN");
        indentLevel++;
        bool result = match("noun");
        if (result) {
            consume();
        }
        indentLevel--;
        return result;
    }
    
    bool verb() {
        debugPrint("Checking for VERB");
        indentLevel++;
        bool result = match("verb");
        if (result) {
            consume();
        }
        indentLevel--;
        return result;
    }

public:
    Parser() : currentToken(0), indentLevel(0) {}
    
    void parse(const string& input) {
        tokens.clear();
        currentToken = 0;
        indentLevel = 0;
        
        cout << "\n========= TOKENIZATION PHASE =========\n" << endl;
        
        // Enhanced tokenization
        string current = input;
        size_t start = 0;
        while ((start = current.find('<', start)) != string::npos) {
            size_t end = current.find('>', start);
            if (end == string::npos) break;
            
            // Extract the content between < and >
            string tokenContent = current.substr(start + 1, end - start - 1);
            
            // Split by comma to separate value and type
            size_t commaPos = tokenContent.find(',');
            if (commaPos != string::npos) {
                string value = tokenContent.substr(0, commaPos);
                string type = tokenContent.substr(commaPos + 1);
                tokens.push_back(Token(type, value));
                cout << "Token found: Type='" << type << "', Value='" << value << "'" << endl;
            }
            
            start = end + 1;
        }
        
        cout << "\n========= PARSING PHASE =========\n" << endl;
        bool isValid = sentence() && currentToken == tokens.size();
        
        cout << "\n========= PARSING RESULT =========\n" << endl;
        if (isValid) {
            cout << "✅ The sentence is syntactically correct!" << endl;
            cout << "All tokens were processed successfully." << endl;
        } else {
            cout << "❌ The sentence is NOT syntactically correct." << endl;
            if (currentToken < tokens.size()) {
                cout << "Parsing stopped at token " << currentToken + 1 << " of " << tokens.size() << endl;
                cout << "Remaining unparsed tokens:" << endl;
                for (size_t i = currentToken; i < tokens.size(); i++) {
                    cout << " - " << tokens[i].type << " ('" << tokens[i].value << "')" << endl;
                }
            }
        }
    }
};

int main() {
    Parser parser;
    string input;
    
    cout << "Enter tokenized sentence (e.g., <the,determiner><dog,noun><caught,verb><a,determiner><cat,noun>):" << endl;
    getline(cin, input);
    
    parser.parse(input);
    return 0;
}