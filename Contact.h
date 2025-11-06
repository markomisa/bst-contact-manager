#ifndef CONTACT_H
#define CONTACT_H

#include <string>

// Structure for a contact
typedef struct Contact {
    std::string name;
    std::string surname;
    long number;
    int x; // Used for share history inactivity count
} Contact;

// Structure for a node in the communication history linked list (MRU/LRU cache)
typedef struct Listnode {
    struct Listnode *next;
    Contact *c;
} Listnode;

// Structure for the header of the communication history linked list
typedef struct ListHeader {
    Listnode *head;
    int curr_size, max_size;
} ListHeader;

// Class for a node in the Binary Search Tree (Contact Directory)
class Treenode {
public:
    Treenode(Contact *c1) {
        c = c1;
        left = right = parent = nullptr;
        id = 0; // Initialize id
    }
    ~Treenode() {
        // Important: We only delete the Contact object if it was allocated 
        // specifically for this Treenode (which is the case in make_tree and add_node).
        // Since Listnode uses these same pointers, there is a risk of double deletion.
        // For simplicity *in this specific refactoring*, we trust the BST owns the memory.
        delete c; 
    }
    Contact* c;
    Treenode *parent;
    Treenode *left;
    Treenode *right;
    int id; // Used for level-order printing
};

#endif // CONTACT_H 