#include <iostream>
#include <fstream>

const int BUFFER_SIZE = 20;

struct BufferNode {
    char buffer[BUFFER_SIZE];
    BufferNode* next;

    BufferNode() : next(nullptr) {
        // Initialize buffer with null terminators
        std::fill(buffer, buffer + BUFFER_SIZE, '\0');
    }
};

int state = 0;

void bufferReader(BufferNode* current, char lexime[], int &bi, int &li) {
    while (current != nullptr && current->buffer[bi] != '\0') {
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

        // Move to the next buffer node if the current buffer is done
        if (bi >= BUFFER_SIZE) {
            current = current->next;
            bi = 0; // Reset buffer index for the next node
        }
    }
}

int main() {
    std::ifstream file("code.txt");  
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file." << std::endl;
        return 1;
    }

    // Start of the buffer linked list
    BufferNode* head = new BufferNode();
    BufferNode* current = head;

    char lex[100] = {0};
    int lexIndex = 0;

    while (!file.eof()) {
        std::cout << "Buffer Called\n";
        
        // Read into the current buffer node
        file.read(current->buffer, BUFFER_SIZE);
        std::size_t bytesRead = file.gcount();

        if (bytesRead == BUFFER_SIZE) {
            // Create a new buffer node if the current one is full
            BufferNode* newNode = new BufferNode();
            current->next = newNode;
            current = newNode;
        }
    }

    // Process the entire linked list of buffers
    int buffIndex = 0;
    bufferReader(head, lex, buffIndex, lexIndex);

    // Clean up the linked list
    current = head;
    while (current != nullptr) {
        BufferNode* temp = current;
        current = current->next;
        delete temp;
    }

    file.close();
    return 0;
}
