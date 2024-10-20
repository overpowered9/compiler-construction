#include <iostream>
#include <fstream>

const int BUFFER_SIZE = 20;

struct BufferNode {
    char buffer[BUFFER_SIZE + 1];  // +1 for null termination
    BufferNode* next;

    BufferNode() : next(nullptr) {
        std::fill(buffer, buffer + BUFFER_SIZE + 1, '\0'); // Fill with nulls
    }
};

int state = 0;

void bufferReader(BufferNode* current, char lexime[], int &bi, int &li) {
    while (bi < BUFFER_SIZE && current->buffer[bi] != '\0') {
        char value = current->buffer[bi];
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

    // Head of the buffer linked list
    BufferNode* head = new BufferNode();
    BufferNode* current = head;

    char lex[100] = {0};
    int lexIndex = 0;

    while (!file.eof()) {
        
        
        // Read from file into current buffer node
        file.read(current->buffer, BUFFER_SIZE);
        std::size_t bytesRead = file.gcount();
        
        // Null-terminate to avoid junk beyond valid content
        current->buffer[bytesRead] = '\0';

        // Process the current buffer
        int buffIndex = 0;
        bufferReader(current, lex, buffIndex, lexIndex);

        if (bytesRead == BUFFER_SIZE) {
            // If we haven't reached the end of the file, create a new buffer node
            std::cout << "newbuffer\n";
            BufferNode* newNode = new BufferNode();
            current->next = newNode;
            current = newNode;
        }
    }

    // Close the file
    file.close();

    // Clean up the linked list
    current = head;
    while (current != nullptr) {
        BufferNode* temp = current;
        current = current->next;
        delete temp;
    }

    return 0;
}
