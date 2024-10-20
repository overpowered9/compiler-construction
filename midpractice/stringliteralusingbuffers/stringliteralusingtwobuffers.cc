#include <iostream>
#include <fstream>
#include <cctype>
#include <string>

using namespace std;

#define BUFFER_SIZE 4026

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
    // Define the transition table for the state machine
    State transitionTable[4][7] = {
        {IN_STRING, START, START, FALSE, FALSE, NONE},
        {END, IN_STRING, ESCAPE, FALSE, FALSE, NONE},
        {END, END, END, TRUE, FALSE, LITERAL},
        {IN_STRING, IN_STRING, IN_STRING, FALSE, FALSE, NONE}};

    char lexeme[100];
    int li = 0;
    int state = 0;

    // Two buffers for reading chunks of the file
    char buffer1[BUFFER_SIZE];
    char buffer2[BUFFER_SIZE];
    bool usingBuffer1 = true;  // Flag to switch between the two buffers

    ifstream file("code.txt", ios::binary);
    if (!file.is_open())
    {
        cerr << "File not found" << endl;
        return 1;
    }

    int size = 0;  // Number of bytes read in the current buffer
    bool fileEnded = false;  // Flag to indicate if the file has been completely read

    // Read the first buffer
    file.read(buffer1, BUFFER_SIZE);
    size = file.gcount();

    // Main loop to process the buffers
    while (size > 0)
    {
        char *currentBuffer = usingBuffer1 ? buffer1 : buffer2;  // Pointer to the active buffer
        for (int i = 0; i < size; i++)
        {
            char c = currentBuffer[i];
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
                lexeme[li] = '\0';  // Null terminate the lexeme
                generateToken(lexeme, transitionTable[state][5]);
                state = 0;  // Reset state for next token
                li = 0;     // Reset lexeme index
            }
        }

        // Check if we need to load the next buffer
        if (!fileEnded)
        {
            // Read the next chunk into the other buffer
            if (usingBuffer1)
            {
                file.read(buffer2, BUFFER_SIZE);
                size = file.gcount();  // Get the number of bytes read
            }
            else
            {
                file.read(buffer1, BUFFER_SIZE);
                size = file.gcount();
            }

            // Switch buffer usage
            usingBuffer1 = !usingBuffer1;

            // If no more bytes were read, mark the file as ended
            if (size == 0)
            {
                fileEnded = true;
            }
        }
    }

    return 0;
}
