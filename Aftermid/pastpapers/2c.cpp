#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <map>
#include <algorithm>
#include <stack>
#include <utility>

using namespace std;

// New data structure to preserve the order of grammar rules
vector<pair<string, vector<vector<string>>>> grammar;
map<string, set<string>> firstSet, followSet;
map<pair<string, string>, vector<string>> parsingTable;

void computeFirst(string nonTerminal);
void computeFollow(string nonTerminal);
void buildParsingTable();
void displayFirstFollow();
void displayParsingTable();
string predictiveParsing(const vector<string> &input);

int main()
{
    grammar.push_back({"Sentence", {{"NounPhrase", "VerbPhrase"}}});
    grammar.push_back({"NounPhrase", {{"Determiner", "Noun", "NounPhrase'"}}});
    grammar.push_back({"VerbPhrase", {{"Verb", "NounPhrase", "VerbPhrase'"}, {"Verb", "VerbPhrase'"}}});
    grammar.push_back({"PrepositionalPhrase", {{"Preposition", "NounPhrase"}}});
    grammar.push_back({"Determiner", {{"the"}, {"a"}}});
    grammar.push_back({"Noun", {{"cat"}, {"dog"}, {"man"}, {"woman"}}});
    grammar.push_back({"Verb", {{"chased"}, {"caught"}}});
    grammar.push_back({"Preposition", {{"in"}, {"on"}, {"with"}}});
    grammar.push_back({"NounPhrase'", {{"PrepositionalPhrase", "NounPhrase'"}, {"eps"}}});
    grammar.push_back({"VerbPhrase'", {{"PrepositionalPhrase", "VerbPhrase'"}, {"eps"}}});

    // grammar.push_back({"E", {{"T", "E'"}}});
    // grammar.push_back({"E'", {{"+", "T", "E'"}, {"eps"}}});
    // grammar.push_back({"T", {{"F", "T'"}}});
    // grammar.push_back({"T'", {{"*", "F", "T'"}, {"eps"}}});
    // grammar.push_back({"F", {{"id"}, {"(", "E", ")"}}});

    cout << "Grammar:\n";
    for (auto &rule : grammar)
    {
        cout << rule.first << " -> ";
        for (auto &production : rule.second)
        {
            for (auto &symbol : production)
                cout << symbol << " ";
            cout << "| ";
        }
        cout << endl;
    }
    // Compute FIRST and FOLLOW sets
    for (auto &rule : grammar)
        computeFirst(rule.first);
    for (auto &rule : grammar)
        computeFollow(rule.first);

    buildParsingTable();

    // vector<string> inputTokens = {"id", "+", "id", "*", "id"};
    // the dog chased a cat
    vector<string> inputTokens = {"the", "dog", "chased", "a", "cat"};
    string parseTree = predictiveParsing(inputTokens);
    cout << "Parse Tree:\n"
         << parseTree;

    return 0;
}

void computeFirst(string nonTerminal)
{
    if (!firstSet[nonTerminal].empty())
        return;

    for (auto &rule : grammar)
    {
        if (rule.first != nonTerminal)
            continue;
        for (auto &production : rule.second)
        {
            for (auto &symbol : production)
            {
                if (find_if(grammar.begin(), grammar.end(), [&](auto &r)
                            { return r.first == symbol; }) == grammar.end())
                { // Terminal
                    firstSet[nonTerminal].insert(symbol);
                    break;
                }
                else
                { // Non-terminal
                    computeFirst(symbol);
                    for (auto &term : firstSet[symbol])
                    {
                        if (term != "eps")
                            firstSet[nonTerminal].insert(term);
                    }
                    if (firstSet[symbol].count("eps") == 0)
                        break;
                }
            }
        }
    }
}

void computeFollow(string nonTerminal)
{
    // If the FOLLOW set is already computed, return
    if (!followSet[nonTerminal].empty())
        return;

    // Special case: Add '$' to the FOLLOW set of the start symbol
    if (nonTerminal == grammar[0].first)
        followSet[nonTerminal].insert("$");

    // Iterate through each rule in the grammar
    for (auto &rule : grammar)
    {
        for (auto &production : rule.second)
        {
            for (size_t i = 0; i < production.size(); ++i)
            {
                if (production[i] == nonTerminal)
                {
                    // If there's a next symbol in the production
                    if (i + 1 < production.size())
                    {
                        string nextSymbol = production[i + 1];

                        // If the next symbol is a terminal, add it to FOLLOW
                        if (find_if(grammar.begin(), grammar.end(), [&](auto &r)
                                    { return r.first == nextSymbol; }) == grammar.end())
                        {
                            followSet[nonTerminal].insert(nextSymbol);
                        }
                        else
                        {
                            // Add FIRST(nextSymbol) to FOLLOW(nonTerminal), excluding eps
                            for (auto &term : firstSet[nextSymbol])
                            {
                                if (term != "eps")
                                {
                                    followSet[nonTerminal].insert(term);
                                }
                            }

                            // If FIRST(nextSymbol) contains eps, propagate FOLLOW(rule head)
                            if (firstSet[nextSymbol].count("eps") > 0)
                            {
                                computeFollow(rule.first); // Recursive call to compute FOLLOW(rule head)
                                for (auto &term : followSet[rule.first])
                                {
                                    followSet[nonTerminal].insert(term);
                                }
                            }
                        }
                    }
                    else
                    {
                        // If nonTerminal is at the end, propagate FOLLOW(rule head)
                        computeFollow(rule.first); // Recursive call to compute FOLLOW(rule head)
                        for (auto &term : followSet[rule.first])
                        {
                            followSet[nonTerminal].insert(term);
                        }
                    }
                }
            }
        }
    }
}

void buildParsingTable()
{
    for (auto &rule : grammar)
    {
        string nonTerminal = rule.first;
        for (auto &production : rule.second)
        {
            set<string> firstSetOfProduction;
            bool containsEpsilon = true;

            for (auto &symbol : production)
            {
                if (symbol == "eps")
                {
                    containsEpsilon = true;
                    break;
                }
                if (find_if(grammar.begin(), grammar.end(), [&](auto &r)
                            { return r.first == symbol; }) == grammar.end())
                { // Terminal
                    firstSetOfProduction.insert(symbol);
                    containsEpsilon = false;
                    break;
                }
                else
                { // Non-terminal
                    for (auto &term : firstSet[symbol])
                    {
                        if (term != "eps")
                            firstSetOfProduction.insert(term);
                    }
                    if (firstSet[symbol].count("eps") == 0)
                    {
                        containsEpsilon = false;
                        break;
                    }
                }
            }

            for (auto &term : firstSetOfProduction)
            {
                parsingTable[{nonTerminal, term}] = production;
            }

            if (containsEpsilon)
            {
                for (auto &term : followSet[nonTerminal])
                {
                    parsingTable[{nonTerminal, term}] = production;
                }
            }
        }
    }
}

void displayFirstFollow()
{
    cout << "FIRST Sets:\n";
    for (auto &rule : firstSet)
    {
        cout << rule.first << ": { ";
        for (auto &term : rule.second)
            cout << term << " ";
        cout << "}\n";
    }

    cout << "\nFOLLOW Sets:\n";
    for (auto &rule : followSet)
    {
        cout << rule.first << ": { ";
        for (auto &term : rule.second)
            cout << term << " ";
        cout << "}\n";
    }
}

void displayParsingTable()
{
    cout << "\nPredictive Parsing Table:\n";
    for (auto &entry : parsingTable)
    {
        cout << entry.first.first << ", " << entry.first.second << " -> ";
        for (auto &symbol : entry.second)
            cout << symbol << " ";
        cout << "\n";
    }
}

string predictiveParsing(const vector<string> &input) 
{
    stack<string> parseStack;          // Parsing stack
    parseStack.push("$");              // End marker
    parseStack.push(grammar[0].first); // Start symbol (LHS of the first grammar rule)

    string parseTree = ""; // Parse tree representation

    vector<string> tokens = input;
    tokens.push_back("$"); // Add end marker to the input stream

    int pointer = 0; // Input pointer

    while (!parseStack.empty()) 
    {
        string top = parseStack.top();
        string currentToken = tokens[pointer];

        // If the top of the stack matches the current token
        if (top == currentToken) 
        {
            parseTree += "Match: " + top + "\n";
            parseStack.pop();
            pointer++;
        } 
        else if (parsingTable.find({top, currentToken}) != parsingTable.end()) 
        {
            // Find production rule from the parsing table
            vector<string> production = parsingTable.at({top, currentToken});
            parseStack.pop(); // Pop the non-terminal from the stack

            if (production[0] != "eps") 
            { 
                // Push production symbols to stack in reverse order
                for (auto it = production.rbegin(); it != production.rend(); ++it) 
                {
                    parseStack.push(*it);
                }
            }

            // Add the production to the parse tree
            parseTree += top + " -> ";
            for (const string &symbol : production) 
            {
                parseTree += symbol + " ";
            }
            parseTree += "\n";
        } 
        else 
        {
            // No matching rule found in parsing table
            cout << "Parsing error: No matching rule for (" << top << ", " << currentToken << ")." << endl;
            break;
        }
    }

    if (!parseStack.empty()) 
    {
        cout << "Parsing failed: Stack is not empty." << endl;
    } 
    else if (pointer != tokens.size() - 1) 
    {
        cout << "Parsing failed: Input not fully consumed." << endl;
    }

    return parseTree;
}