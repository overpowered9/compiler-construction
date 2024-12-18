#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

#define BUFFER_SIZE 512

enum State
{
    START,
    IN_STRING,
    ESCAPE,
    END,
};

enum COL
{
    QUOTE,
    NOT_QUOTE_SLASH,
    SLASH,
    ESCAPE_CHAR,
    ACCEPTING,
    RETRATING,
};

int getCharType(char c)
{
    if (c == 34)
        return 0;
    else if (c == '\\')
        return 2;
    else if (c == 'n' || c == 't' || c == 'b' || 'r')
        return 3;
    else
        return 1;
}

int main()
{
    int transitionTable[4][7] = {

        {IN_STRING, START, START, START, 0, 0},
        {END, IN_STRING, ESCAPE, IN_STRING, 0, 0},
        {IN_STRING, -1, IN_STRING, IN_STRING, 0, 0},
        {-1, -1, -1, -1, 1, 0},
    };

    char buffer1[BUFFER_SIZE];
    char buffer2[BUFFER_SIZE];
    char *activeBuffer = buffer1;
    char *inactiveBuffer = buffer2;

    char lexiam[100];
    int li = 0;
    int state = 0;

    ifstream inputFile("p3.cpp", ios::binary);
    if (!inputFile.is_open())
    {
        cerr << "File is not \nOpen\n\n\t" << endl;
        return 1;
    }
    ofstream outputFile("output.cpp", ios::binary);
    if (!outputFile.is_open())
    {
        cerr << "Output\t File is not Open" << endl;
        return 1;
    }

    while (true)
    {
        inputFile.read(activeBuffer, BUFFER_SIZE);
        int byteRead = inputFile.gcount();

        if (byteRead == 0)
        {
            break;
        }

        for (size_t i = 0; i <= byteRead; i++)
        {
            char c = activeBuffer[i];
            int prevState = state;
            state = transitionTable[state][getCharType(c)];
            // cout << c << " " << state << endl;

            if (state != 0 && prevState != 0 && state != 3)
            {
                lexiam[li++] = c;
                activeBuffer[i] = ' ';
            }
            if (state == -1)
            {
                cerr << "Invalid Code" << endl;
                return 1;
            }

            if (transitionTable[state][4])
            {
                if (transitionTable[state][5])
                {
                    li--;
                    i--;
                }
                lexiam[li] = '\0';
                cout << lexiam << endl;
                state = 0;
                li = 0;
            }
        }

        outputFile.write(activeBuffer, byteRead);

        char *temp = activeBuffer;
        activeBuffer = inactiveBuffer;
        inactiveBuffer = temp;

        if (inputFile.eof())
        {
            break;
        }
    }

    inputFile.close();
    outputFile.close();
    return 0;
}