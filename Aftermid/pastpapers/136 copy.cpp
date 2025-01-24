#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <regex>

using namespace std;

// Debug function to print stack contents
void printStack(stack<string> s) {
    cout << "Stack contents: ";
    vector<string> items;
    while (!s.empty()) {
        items.push_back(s.top());
        s.pop();
    }
    for (auto it = items.rbegin(); it != items.rend(); ++it) {
        cout << *it << " ";
    }
    cout << "\n";
}

vector<string> tokenize(string input)
{
    vector<string> tokens;
    regex tokenPattern("<([^,]+),([^>]+)>");
    
    cout << "Tokenizing input...\n";
    smatch match;
    while (regex_search(input, match, tokenPattern)) {
        string token = match[1].str();
        tokens.push_back(token);
        cout << "Found token: " << token << "\n";
        input = match.suffix().str();
    }
    return tokens;
}

// Updated parsing table to include PrepositionalPhrase rules
map<string, map<string, vector<string>>> parsingTable = {
    {"Sentence", {
        {"the", {"NounPhrase", "VerbPhrase", "."}}
    }},
    {"NounPhrase", {
        {"the", {"Determiner", "Noun", "OptionalPrepPhrase"}},
        {"a", {"Determiner", "Noun", "OptionalPrepPhrase"}}
    }},
    {"OptionalPrepPhrase", {
        {"with", {"PrepositionalPhrase"}},
        {"in", {"PrepositionalPhrase"}},
        {"on", {"PrepositionalPhrase"}},
        {"caught", {"ε"}},  // epsilon for when there's no PrepPhrase
        {"chased", {"ε"}},
        {".", {"ε"}}
    }},
    {"PrepositionalPhrase", {
        {"with", {"Preposition", "NounPhrase"}},
        {"in", {"Preposition", "NounPhrase"}},
        {"on", {"Preposition", "NounPhrase"}}
    }},
    {"VerbPhrase", {
        {"caught", {"Verb", "NounPhrase"}},
        {"chased", {"Verb", "NounPhrase"}}
    }},
    {"Determiner", {
        {"the", {"the"}},
        {"a", {"a"}}
    }},
    {"Noun", {
        {"dog", {"dog"}},
        {"cat", {"cat"}},
        {"man", {"man"}},
        {"woman", {"woman"}}
    }},
    {"Verb", {
        {"caught", {"caught"}},
        {"chased", {"chased"}}
    }},
    {"Preposition", {
        {"in", {"in"}},
        {"on", {"on"}},
        {"with", {"with"}}
    }}
};

bool predictiveParse(vector<string> tokens)
{
    stack<string> parseStack;
    parseStack.push("$");
    parseStack.push("Sentence");
    
    size_t tokenIndex = 0;
    cout << "\n=== Starting Parse ===\n";
    cout << "Initial stack: Sentence $\n";

    while (!parseStack.empty())
    {
        string top = parseStack.top();
        parseStack.pop();
        
        cout << "\nCurrent token: " << tokens[tokenIndex] << "\n";
        cout << "Current stack top: " << top << "\n";
        printStack(parseStack);

        // Handle epsilon productions
        if (top == "ε") {
            cout << "Applying epsilon production (empty string)\n";
            continue;
        }

        if (top == tokens[tokenIndex])
        {
            cout << "Matched terminal: " << top << "\n";
            tokenIndex++;
            if (tokenIndex >= tokens.size()) {
                cout << "Reached end of input\n";
                break;
            }
        }
        else if (parsingTable.find(top) != parsingTable.end())
        {
            if (parsingTable[top].find(tokens[tokenIndex]) != parsingTable[top].end())
            {
                vector<string> production = parsingTable[top][tokens[tokenIndex]];
                cout << "Applying production " << top << " -> ";
                for (auto &sym : production)
                    cout << sym << " ";
                cout << "\n";

                for (auto it = production.rbegin(); it != production.rend(); ++it)
                {
                    parseStack.push(*it);
                }
            }
            else
            {
                cout << "Error: No production found for non-terminal '" << top 
                     << "' with input token '" << tokens[tokenIndex] << "'\n";
                return false;
            }
        }
        else
        {
            cout << "Error: Unexpected symbol '" << top << "' on stack\n";
            return false;
        }
    }

    if (tokenIndex == tokens.size()) {
        cout << "\nParsing completed successfully\n";
        return true;
    } else {
        cout << "\nParsing failed: Not all input consumed\n";
        return false;
    }
}

int main()
{
    string input;
    cout << "Enter a sentence as tokens: ";
    getline(cin, input);

    vector<string> tokens = tokenize(input);
    tokens.push_back("$");

    cout << "\nTokenized input: ";
    for (const auto& token : tokens) {
        cout << token << " ";
    }
    cout << "\n";

    bool result = predictiveParse(tokens);
    cout << "\nFinal result: " << (result ? "Valid" : "Invalid") << " sentence\n";

    return 0;
}