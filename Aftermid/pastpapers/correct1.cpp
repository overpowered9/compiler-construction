#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <map>
#include <sstream>

using namespace std;

// Function to split the input into tokens
vector<string> tokenize(string input) {
    vector<string> tokens;
    stringstream ss(input);
    string token;
    while (getline(ss, token, ' ')) {
        tokens.push_back(token);
    }
    return tokens;
}

// CFG rules
map<string, vector<vector<string>>> grammar = {
    {"Sentence", {{"NounPhrase", "VerbPhrase", "."}}},
    {"NounPhrase", {{"Determiner", "Noun"}, {"NounPhrase", "PrepositionalPhrase"}}},
    {"VerbPhrase", {{"Verb", "NounPhrase"}, {"VerbPhrase", "PrepositionalPhrase"}, {"Verb"}}},
    {"PrepositionalPhrase", {{"Preposition", "NounPhrase"}}},
    {"Determiner", {{"the"}, {"a"}}},
    {"Noun", {{"dog"}, {"cat"}, {"man"}, {"woman"}}},
    {"Verb", {{"caught"}, {"chased"}}},
    {"Preposition", {{"in"}, {"on"}, {"with"}}},
};

// Predictive parsing table (simplified for provided CFG)
map<string, map<string, vector<string>>> parsingTable = {
    {"Sentence", {{"the", {"NounPhrase", "VerbPhrase", "."}},
                  {"a", {"NounPhrase", "VerbPhrase", "."}}}},
    {"NounPhrase", {{"the", {"Determiner", "Noun"}},
                    {"a", {"Determiner", "Noun"}},
                    {"dog", {"Determiner", "Noun"}},
                    {"cat", {"Determiner", "Noun"}}}},
    {"VerbPhrase", {{"caught", {"Verb", "NounPhrase"}},
                    {"chased", {"Verb", "NounPhrase"}}}},
    {"Determiner", {{"the", {"the"}},
                    {"a", {"a"}}}},
    {"Noun", {{"dog", {"dog"}},
              {"cat", {"cat"}},
              {"man", {"man"}},
              {"woman", {"woman"}}}},
    {"Verb", {{"caught", {"caught"}},
              {"chased", {"chased"}}}},
    {"Preposition", {{"in", {"in"}},
                     {"on", {"on"}},
                     {"with", {"with"}}}},
};

// Function to perform predictive parsing
bool predictiveParse(vector<string> tokens) {
    stack<string> parseStack;
    parseStack.push("Sentence"); // Start with the start symbol

    size_t tokenIndex = 0;
    cout << "Initial Stack: Sentence\n";

    while (!parseStack.empty()) {
        string top = parseStack.top();
        parseStack.pop();

        if (top == tokens[tokenIndex]) {
            // Match terminal
            cout << "Matched: " << top << "\n";
            tokenIndex++;
            if (tokenIndex >= tokens.size()) break;
        } else if (parsingTable.find(top) != parsingTable.end()) {
            // Non-terminal: apply rule
            if (parsingTable[top].find(tokens[tokenIndex]) != parsingTable[top].end()) {
                vector<string> production = parsingTable[top][tokens[tokenIndex]];
                cout << "Expanding " << top << " -> ";
                for (auto &sym : production) cout << sym << " ";
                cout << "\n";

                for (auto it = production.rbegin(); it != production.rend(); ++it) {
                    parseStack.push(*it);
                }
            } else {
                cout << "Error: Unexpected token '" << tokens[tokenIndex] << "' for non-terminal '" << top << "'\n";
                return false;
            }
        } else {
            // Error: No match
            cout << "Error: Unmatched token '" << tokens[tokenIndex] << "'\n";
            return false;
        }
    }

    if (tokenIndex == tokens.size()) {
        cout << "Parsing Successful: Sentence is syntactically correct.\n";
        return true;
    } else {
        cout << "Parsing Failed: Extra tokens remaining.\n";
        return false;
    }
}

int main() {
    string input;
    cout << "Enter a sentence as tokens (e.g., 'the dog caught a cat'): ";
    getline(cin, input);

    vector<string> tokens = tokenize(input);
    tokens.push_back("."); // Add end of sentence marker

    cout << "\nParsing trace:\n";
    bool result = predictiveParse(tokens);

    if (result) {
        cout << "\nThe sentence is syntactically correct.\n";
    } else {
        cout << "\nThe sentence is syntactically incorrect.\n";
    }

    return 0;
}
