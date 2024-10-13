#include <iostream>
using namespace std;

class InnerNode {
public:
    int data;
    InnerNode* next;
    InnerNode* prev;

    InnerNode(int val) {
        data = val;
        next = prev = nullptr;
    }
};

class OuterNode {
public:
    InnerNode* innerHead;
    InnerNode* innerTail; // To keep track of the end of the inner list
    OuterNode* next;
    OuterNode* prev;

    OuterNode() {
        innerHead = innerTail = nullptr;
        next = prev = nullptr;
    }
};

class DoublyLinkedList2D {
public:
    OuterNode* outerHead;
    OuterNode* outerTail;  // To keep track of the end of the outer list

    DoublyLinkedList2D() {
        outerHead = outerTail = nullptr;
    }

    // Function to add a new row (outer list)
    void addRow() {
        OuterNode* newOuterNode = new OuterNode();

        if (!outerHead) {  // Empty list, set head and tail to new node
            outerHead = outerTail = newOuterNode;
        } else {  // Append to the end of the list
            outerTail->next = newOuterNode;
            newOuterNode->prev = outerTail;
            outerTail = newOuterNode;  // Update tail
        }
    }

    // Function to add a new column (inner list) in a specific row
    void addColumn(int row, int data) {
        OuterNode* rowPointer = outerHead;
        for (int i = 0; i < row && rowPointer; i++) {
            rowPointer = rowPointer->next;
        }
        if (!rowPointer) {
            cout << "Row " << row << " doesn't exist!\n";
            return;
        }

        InnerNode* newInnerNode = new InnerNode(data);

        if (!rowPointer->innerHead) {  // If inner list is empty
            rowPointer->innerHead = rowPointer->innerTail = newInnerNode;
        } else {  // Append to the end of the inner list
            rowPointer->innerTail->next = newInnerNode;
            newInnerNode->prev = rowPointer->innerTail;
            rowPointer->innerTail = newInnerNode;  // Update inner tail
        }
    }

    // Function to print the 2D doubly linked list
    void printGrid() {
        OuterNode* row = outerHead;
        while (row) {
            InnerNode* col = row->innerHead;
            while (col) {
                cout << col->data << " ";
                col = col->next;
            }
            cout << endl;
            row = row->next;
        }
    }
};

int main() {
    DoublyLinkedList2D grid;
    
    // Add 3 rows (outer list)
    grid.addRow();
    grid.addRow();
    grid.addRow();
    
    // Add columns (inner list) to each row
    grid.addColumn(0, 1);
    grid.addColumn(0, 2);
    grid.addColumn(0, 3);
    
    grid.addColumn(1, 4);
    grid.addColumn(1, 5);
    grid.addColumn(1, 6);
    
    grid.addColumn(2, 7);
    grid.addColumn(2, 8);
    grid.addColumn(2, 9);

    // Print the 2D linked list
    grid.printGrid();
    
    return 0;
}