#include <iostream>
#include <fstream>
#include <vector>

#define BUFFER_SIZE 4096

// Buffer Block structure for dynamic buffering
struct BufferBlock {
    char data[BUFFER_SIZE];
    int pos;
    BufferBlock* next;

    BufferBlock() : pos(0), next(nullptr) {}
};

// Lexical Analyzer class
class LexicalAnalyzer {
private:
    BufferBlock* currentBlock;
    std::ifstream file;
    bool endOfFile;

    // Load a block of 4096 bytes into the buffer
    BufferBlock* loadBufferBlock() {
        BufferBlock* block = new BufferBlock();
        file.read(block->data, BUFFER_SIZE);
        block->pos = file.gcount();  // Set the block size based on read bytes

        if (block->pos == 0) {
            delete block;
            return nullptr;
        }
        return block;
    }

    // Move to the next buffer block when the current one is exhausted
    void nextBufferBlock() {
        if (currentBlock != nullptr) {
            BufferBlock* temp = currentBlock;
            currentBlock = currentBlock->next;
            delete temp;
        }
        if (currentBlock == nullptr && !endOfFile) {
            currentBlock = loadBufferBlock();
            if (currentBlock == nullptr) {
                endOfFile = true;
            }
        }
    }

public:
    LexicalAnalyzer(const std::string& filename) : currentBlock(nullptr), endOfFile(false) {
        file.open(filename, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file." << std::endl;
            exit(1);
        }
        currentBlock = loadBufferBlock();  // Load the first block
    }

    ~LexicalAnalyzer() {
        while (currentBlock != nullptr) {
            nextBufferBlock();
        }
        if (file.is_open()) {
            file.close();
        }
    }

    // Process the file and detect comments
    void analyze() {
        std::string lexeme = "";
        char currentChar;
        enum State { NORMAL, SINGLE_LINE_COMMENT, MULTI_LINE_COMMENT };
        State state = NORMAL;

        while (!endOfFile || currentBlock->pos > 0) {
            // Get the next character from the buffer
            if (currentBlock->pos > 0) {
                currentChar = currentBlock->data[BUFFER_SIZE - currentBlock->pos];
                currentBlock->pos--;
            } else {
                nextBufferBlock();
                continue;
            }

            switch (state) {
                case NORMAL:
                    if (currentChar == '/') {
                        lexeme += currentChar;
                        if (currentBlock->pos > 0) {
                            char nextChar = currentBlock->data[BUFFER_SIZE - currentBlock->pos];
                            if (nextChar == '/') {
                                state = SINGLE_LINE_COMMENT;
                                lexeme += nextChar;
                                currentBlock->pos--;
                            } else if (nextChar == '*') {
                                state = MULTI_LINE_COMMENT;
                                lexeme += nextChar;
                                currentBlock->pos--;
                            } else {
                                lexeme.clear();  // Reset lexeme if not a comment
                            }
                        }
                    }
                    break;

                case SINGLE_LINE_COMMENT:
                    if (currentChar == '\n') {
                        std::cout << "Single-line comment: " << lexeme << std::endl;
                        lexeme.clear();
                        state = NORMAL;
                    } else {
                        lexeme += currentChar;
                    }
                    break;

                case MULTI_LINE_COMMENT:
                    lexeme += currentChar;
                    if (currentChar == '*' && currentBlock->pos > 0) {
                        char nextChar = currentBlock->data[BUFFER_SIZE - currentBlock->pos];
                        if (nextChar == '/') {
                            lexeme += nextChar;
                            currentBlock->pos--;
                            std::cout << "Multi-line comment: " << lexeme << std::endl;
                            lexeme.clear();
                            state = NORMAL;
                        }
                    }
                    break;
            }
        }

        if (!lexeme.empty()) {
            if (state == SINGLE_LINE_COMMENT) {
                std::cout << "Single-line comment (incomplete): " << lexeme << std::endl;
            } else if (state == MULTI_LINE_COMMENT) {
                std::cout << "Multi-line comment (incomplete): " << lexeme << std::endl;
            }
        }
    }
};

// Main function to drive the analyzer
int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <source_code_file>" << std::endl;
        return 1;
    }

    LexicalAnalyzer analyzer(argv[1]);
    analyzer.analyze();

    return 0;
}
