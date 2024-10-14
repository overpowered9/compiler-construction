#include <iostream>
#include <fstream>
#include <cctype>
#include <string>

using namespace std;

#define BUFFER_SIZE 1024

enum State
{
    START,
    IN_STRING,
    END,
    ESCAPE,
    FALSE,
    TRUE,
    NONE,
    LITERAL
};

int getCharType(char c)
{
    if (c == '"')
        return 0;
    else if (c != '"')
        return 1;
    else if (c == '\\')
        return 2;
    else
        return 3;
}

void generateToken(const char *lexeme, State state)
{
    if (state == LITERAL)
    {
        cout << "Token: " << lexeme << ", Token Type: " << "LITERAL" << endl;
    }
}

int main()
{
    State transitionTable[4][7] = {

        {IN_STRING, START, START, FALSE, FALSE, NONE},
        {END, IN_STRING, ESCAPE, FALSE, FALSE, NONE},
        {END, END, END, TRUE, FALSE, LITERAL},
        {IN_STRING, IN_STRING, IN_STRING, FALSE, FALSE, NONE}};

    char lexeme[100];
    int li = 0;
    int state = 0;
    char buffer[BUFFER_SIZE];

    ifstream file("code.txt", ios::binary);
    if (!file.is_open())
    {
        cerr << "File not found" << endl;
        return 1;
    }

    file.read(buffer, BUFFER_SIZE);
    int size = file.gcount();


    for (int i = 0; i < size; i++)
    {
        char c = buffer[i];
        if (transitionTable[state][3] == FALSE)
        {
            state = transitionTable[state][getCharType(c)];
            if (state != 0)
            {
                lexeme[li] = c;
                li++;
            }
        }
        else
        {
            lexeme[li] = '\0';
            generateToken(lexeme, transitionTable[state][5]);
            state = 0;
            li = 0;
        }
    }
    return 0;
}