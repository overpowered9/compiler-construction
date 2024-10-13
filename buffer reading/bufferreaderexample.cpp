#include <iostream>
#include <fstream>

int state = 0;
char buffer[20];  
int buffIndex = 0;

void bufferReader(char buffer[], char lexime[], int &bi, int &li) {
    while (buffer[bi] != '\0') {
        char value = buffer[bi];
        switch (state) {
            case 0:
                if ((value >= '0' && value <= '9') || (value == '_')) {
                    state = 1;
                    lexime[li] = value;
                    li++;
                    std::cout << "State->1, lexime: " << lexime << std::endl;
                }
                break;

            case 1:
                if ((value >= 'a' && value <= 'z') || (value >= '0' && value <= '9') || (value == '_')) {
                    lexime[li] = value;
                    li++;
                    std::cout << "State 1, lexime: " << lexime << std::endl;
                } else {
                    state = 2;
                    lexime[li] = '\0'; // End of lexeme
                    std::cout << "State changed to 2, lexime: " << lexime << std::endl;
                    li = 0; // Reset lexime index for next lexeme
                }
                break;

            case 2:
                state = 0;
                bi--; // Step back 
                break;
        }
        bi++;
    }
}

int main() {
    std::ifstream file("code.txt");  
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file." << std::endl;
        return 1;
    }

    char lex[100] = {0};
    int lexIndex = 0;

    while (!file.eof()) {
        // 
        std::cout << "Buffer Called\n";
        file.read(buffer, sizeof(buffer) - 1);
        buffer[file.gcount()] = '\0';  // Null

        // Process the current buffer
        buffIndex = 0;
        bufferReader(buffer, lex, buffIndex, lexIndex);
    }

    file.close();
    return 0;
}
