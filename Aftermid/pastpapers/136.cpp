#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <regex>

using namespace std;

vector<string> maketokens(string input)
{
    vector<string> tokens;
    // Regex pattern to match <word,category>
    regex tokenPattern("<([^,]+),([^>]+)>");

    smatch match;
    while (regex_search(input, match, tokenPattern)) {
        // Extract token (word) and category
        string token = match[1].str();
        tokens.push_back(token);
        input = match.suffix().str();
    }
    return tokens;
}



map<string, map<string, vector<string>>> parsingTable = {
    {"Sentence", {{"the", {"NounPhrase", "VerbPhrase", "."}}, {"a", {"NounPhrase", "VerbPhrase", "."}}}},
    {"NounPhrase", {{"the", {"Determiner", "Noun"}}, {"a", {"Determiner", "Noun"}}, {"dog", {"Determiner", "Noun"}}, {"cat", {"Determiner", "Noun"}}}},
    {"VerbPhrase", {{"caught", {"Verb", "NounPhrase"}}, {"chased", {"Verb", "NounPhrase"}}}},
    {"Determiner", {{"the", {"the"}}, {"a", {"a"}}}},
    {"Noun", {{"dog", {"dog"}}, {"cat", {"cat"}}, {"man", {"man"}}, {"woman", {"woman"}}}},
    {"Verb", {{"caught", {"caught"}}, {"chased", {"chased"}}}},
    {"Preposition", {{"in", {"in"}}, {"on", {"on"}}, {"with", {"with"}}}},
};

// predictingfunction
bool predictiveParsingFunctione(vector<string> tokens)
{
    stack<string> parseStack;
    parseStack.push("$");
    parseStack.push("Sentence");

    size_t tokenIndex = 0;
    cout << "Initial Stack: Sentence\n";

    while (!parseStack.empty())
    {
        string top = parseStack.top();
        parseStack.pop();

        if (top == tokens[tokenIndex])
        {
            if (top == "$")
            {
                cout << "$ and $ matched.\n";
            }

            // Match terminal
            cout << "Token: " << tokens[tokenIndex] << "\n";
            cout << "Matched: " << top << "\n";
            tokenIndex++;
            if (tokenIndex >= tokens.size())
                break;
        }
        else if (parsingTable.find(top) != parsingTable.end())
        {
            // Non-terminal: apply rule
            if (parsingTable[top].find(tokens[tokenIndex]) != parsingTable[top].end())
            {
                cout << "Token: " << tokens[tokenIndex] << "\n";
                vector<string> production = parsingTable[top][tokens[tokenIndex]];
                cout << "Expanding " << top << " -> ";
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
                cout << "Error: Unexpected token '" << tokens[tokenIndex] << "' for non-terminal '" << top << "'\n";
                return false;
            }
        }
        else
        {
            // Error: No match
            cout << "Error: Unmatched token '" << tokens[tokenIndex] << "'\n";
            return false;
        }
    }

    if (tokenIndex == tokens.size())
    {
        cout << "Parsing Successful: Sentence is syntactically correct.\n";
        return true;
    }
    else
    {
        cout << "Parsing Failed: Extra tokens remaining.\n";
        return false;
    }
}

int main()
{
    string input;
    cout << "Enter a sentence as tokens (forexample., '<the,determiner><dog,noun><caught,verb><a,determiner><cat,noun><.,punctuation>'): ";
    getline(cin, input);

    vector<string> tokens = maketokens(input);
    tokens.push_back("$");

    cout << "\nTracing Parse:\n";
    bool result = predictiveParsingFunctione(tokens);

    if (result)
    {
        cout << "\nThe sentence is valid syntactically.\n";
    }
    else
    {
        cout << "\nThe sentence is incorrect syntactically .\n";
    }

    return 0;
}