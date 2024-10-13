#include <iostream>

int state = 0;
char buffer[] = "_int_main(){} ";
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
            // Remain in state 1
                if ((value >= 'a' && value <= 'z') || (value >= '0' && value <= '9') || (value == '_')) {
                    
                    lexime[li] = value;
                    li++;
                    std::cout << "State 1, lexime: " << lexime << std::endl;
                } else {
                    // Move to final state 2
                    state = 2;
                    lexime[li] = '\0'; // End of lexeme
                    std::cout << "State changed to 2, lexime: " << lexime << std::endl;
                    li = 0; // Reset lexime index for next lexeme
                }
                break;

            case 2:
                // Reset to start new lexeme
                state = 0;
                bi--; // Step back to re-process this character
                break;
        }
        bi++;
    }
}

int main() {
    char lex[100] = {0};
    int lexIndex = 0;

    bufferReader(buffer, lex, buffIndex, lexIndex);

    return 0;
}
