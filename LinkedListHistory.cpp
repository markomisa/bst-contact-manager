#include "LinkedListHistory.h"
#include <iostream>
#include <cstdlib>

using namespace std;

// Helper: Finds the node BEFORE the contact, or nullptr if contact is head or not found
static Listnode* find_in_history(ListHeader* history, Contact* c) {
    Listnode* prev = nullptr;
    Listnode* current = history->head;

    while (current != nullptr) {
        if (current->c->name == c->name && current->c->surname == c->surname && current->c->number == c->number) {
            return prev;
        }
        prev = current;
        current = current->next;
    }
    return nullptr;
}

// Helper: Moves a found node to the front of the list
static void move_to_front(ListHeader* history, Listnode* prev, Listnode* current) {
    if (current == history->head) return; // Already at front

    if (prev != nullptr) {
        prev->next = current->next; // Bypass the current node
        current->next = history->head; // Link current to old head
        history->head = current; // Make current the new head
    }
}

// Helper: Deletes the last node of the list
static void delete_list_tail(ListHeader *l) {
    if (!l->head) return;
    
    Listnode *temp = l->head;
    Listnode *prev_tail = nullptr;

    if (!temp->next) { // Only one node
        delete temp;
        l->head = nullptr;
        l->curr_size = 0;
        return;
    }

    while (temp->next != nullptr) {
        prev_tail = temp;
        temp = temp->next;
    }

    // Remove the tail
    delete temp;
    prev_tail->next = nullptr;
    l->curr_size--;
}

ListHeader *new_list_header() {
    ListHeader *l = new ListHeader;
    l->head = nullptr;
    l->curr_size = 0;
    l->max_size = 0; 
    return l;
}

void komuniciraj(ListHeader* history, Contact* c) {
    if (history->max_size <= 0) return; 

    Listnode* prev = find_in_history(history, c);
    
    // Check if contact is found (prev points to the node before it, or it's the head)
    bool is_found = false;
    Listnode* current = nullptr;
    
    if (history->head != nullptr && history->head->c->name == c->name && history->head->c->surname == c->surname && history->head->c->number == c->number) {
        is_found = true;
        current = history->head;
    } else if (prev != nullptr) {
        is_found = true;
        current = prev->next;
    }

    if (is_found) {
        move_to_front(history, prev, current);
        return;
    }

    // Contact not found, must add it to the front
    if (history->curr_size == history->max_size) {
        // List is full, remove the tail
        delete_list_tail(history);
    }

    // Add new node to the front
    Listnode* new_node = new Listnode;
    new_node->c = c; // Uses the existing Contact* from the BST (no copy, just pointer)
    new_node->next = history->head;
    history->head = new_node;
    history->curr_size++;
}

static void print_listnode(Listnode *node){
    cout << "prezime: " << node->c->surname
    << ", ime: " << node->c->name
    << ", telefonski broj: " << node->c->number << endl;
}

void print_history(ListHeader *l) {
    if(!l->head) {
        cout << "Trenutno nema istorije komunikacije." << endl;
    }
    else {
        cout << "\n--- Trenutna Istorija Komunikacije (Max: " << l->max_size << ", Current: " << l->curr_size << ") ---" << endl;
        Listnode *curr = l->head;
        int i = 1;
        while (curr) {
            cout << i++ << ". ";
            print_listnode(curr);
            curr = curr->next;
        }
    }
    cout << endl;
}