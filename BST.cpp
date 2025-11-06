#include "BST.h"
#include <iostream>
#include <stack>
#include <queue>
#include <tuple>
#include <algorithm> // For std::sort

using namespace std;

// Comparator for sorting contacts (Surname then Name)
bool compareContacts(Contact* a, Contact* b) {
    if (a->surname != b->surname) {
        return a->surname < b->surname;
    }
    return a->name < b->name;
}

// --- Helper Functions ---

Treenode *bst_min(Treenode *root) {
    Treenode *p = root;
    while (p && p->left != nullptr) {
        p = p->left;
    }
    return p;
}

Treenode *bst_max(Treenode *root) {
    Treenode *p = root;
    while (p && p->right != nullptr) {
        p = p->right;
    }
    return p;
}

Treenode *find_succ(Treenode *r) {
    Treenode *p = r;
    Treenode *q;
    if(p->right != nullptr) {
        return bst_min(p->right);
    }
    else {
        q = p->parent;
        while ((q != nullptr) && (p == q->right)) {
            p = q;
            q = q->parent;
        }
        return q;
    }
}

Treenode *find_pred(Treenode *r) {
    Treenode *p = r;
    Treenode *q;
    if(p->left != nullptr) {
        return bst_max(p->left);
    }
    else {
        q = p->parent;
        while ((q != nullptr) && (p == q->left)) {
            p = q;
            q = q->parent;
        }
        return q;
    }
}

Contact *make_contact(const string& name, const string& surname, long number) {
    Contact *c = new Contact;
    c->name = name;
    c->surname = surname;
    c->number = number;
    c->x = 0;
    return c;
}

void print_node(Treenode *node){
    if (!node) return;
    cout << "prezime: " << node->c->surname
    << ", ime: " << node->c->name
    << ", telefonski broj: " << node->c->number << endl;
}

void print_vector(const vector<Treenode*>& nodes) {
    if (nodes.empty()) {
        cout << "Nema rezultata pretrage." << endl;
        return;
    }
    cout << "\n--- Rezultati pretrage ---" << endl;
    for(size_t i = 0; i < nodes.size(); i++) {
        cout << i << ": ";
        print_node(nodes[i]);
    }
    cout << "--------------------------" << endl;
}

// --- Core BST Functions ---

// Creates a balanced BST from a sorted vector using the median of the current range.
Treenode *make_tree(vector<Contact*>& contacts){
    // Sort contacts first to ensure a balanced tree structure from the array
    std::sort(contacts.begin(), contacts.end(), compareContacts);

    if (contacts.empty()) return nullptr;
    
    Treenode *parent_temp;
    // Stack stores {parent_node, low_index, high_index} for the RIGHT subarray
    stack <tuple<Treenode*, int, int> > st;
    int n = contacts.size();
    int low = 0, high = n - 1;
    int mid = (low + high) / 2;

    // Root node
    Treenode *root = new Treenode(contacts[mid]);
    root->parent = nullptr;
    Treenode *next = root;
    
    // Push the right subarray of the root
    st.push(make_tuple(next, mid + 1, high));
    high = mid - 1; // New range for the left side of the root

    // Build left side iteratively (simulating recursion with a stack)
    while(low <= high)
    {
        parent_temp = next;
        mid = (low + high) / 2;
        next->left = new Treenode(contacts[mid]);
        next = next->left;
        next->parent = parent_temp;
        // Push the right subarray of the current node
        st.push(make_tuple(next, mid + 1, high));
        high = mid - 1; // Continue left
    }
    if (next) next->left = nullptr;

    // Process stack to build the right subtrees
    while (!(st.empty()))
    {
        tuple<Treenode*, int, int> t = st.top();
        st.pop();
        next = get<0>(t); 
        low = get<1>(t); 
        high = get<2>(t);
        
        if (low <= high)
        {
            parent_temp = next;
            mid = (low + high)/2;
            next->right = new Treenode(contacts[mid]);
            next = next->right;
            next->parent = parent_temp;
            
            // Push the right subarray of the new right child
            st.push(make_tuple(next, mid + 1, high));
            high = mid - 1; // Start building the left subtree of the new right child

            // Build left side iteratively
            while(low <= high)
            {
                parent_temp = next;
                mid = (low + high) / 2;
                next->left = new Treenode(contacts[mid]);
                next = next->left;
                next->parent = parent_temp;
                st.push(make_tuple(next, mid + 1, high));
                high = mid - 1;
            }
            if (next) next->left = nullptr;
        }
        else if (next) next->right = nullptr; // Ensure end of branch is null
    }
    return root;
}

void add_node(Treenode *root, Treenode *new_node) {
    if (!root) return; // Cannot add to null tree (caller should handle root)

    Treenode *p = root, *q = nullptr;
    while (p != nullptr) {
        q = p;
        if (new_node->c->surname < p->c->surname) {
            p = p->left;
        } else if (new_node->c->surname > p->c->surname) {
            p = p->right;
        } else { // Same surname, compare by name
            if (new_node->c->name < p->c->name) {
                p = p->left;
            } else {
                p = p->right;
            }
        }
    }

    if (new_node->c->surname < q->c->surname) {
        q->left = new_node;
    } else if (new_node->c->surname > q->c->surname) {
        q->right = new_node;
    } else {
        if (new_node->c->name < q->c->name) {
            q->left = new_node;
        } else {
            q->right = new_node;
        }
    }
    new_node->parent = q;
}

// Function to handle reparenting after deletion (utility)
static void transplant(Treenode *&root, Treenode *u, Treenode *v) {
    if (u->parent == nullptr) {
        root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    if (v != nullptr) {
        v->parent = u->parent;
    }
}

void delete_node(Treenode *&root, const string& surname, const string& name) {
    if (!root) return;

    int pos = 0;
    vector<Treenode*> candidates = bin_by_name_and_surname(name, surname, root);
    if (candidates.empty()) {
        cout << "Ne mozete obrisati nepostojeci kontakt" << endl;
        return;
    }
    print_vector(candidates);
    if (candidates.size() >= 2) {
        cout << "\nKoji kontakt zelite da obrisete? Unesite redni broj (0 - " << candidates.size() - 1 << "): ";
        cin >> pos;
        if (pos < 0 || pos >= candidates.size()) {
            cout << "Neispravan odabir." << endl;
            return;
        }
    }
    Treenode *brisem = candidates[pos];
    Treenode *y = brisem;

    if (brisem->left == nullptr) {
        transplant(root, brisem, brisem->right);
    } else if (brisem->right == nullptr) {
        transplant(root, brisem, brisem->left);
    } else {
        Treenode *y = bst_min(brisem->right);
        if (y->parent != brisem) {
            transplant(root, y, y->right);
            y->right = brisem->right;
            y->right->parent = y;
        }
        transplant(root, brisem, y);
        y->left = brisem->left;
        y->left->parent = y;
    }
    
    delete brisem;
    cout << "Uspesno obrisan kontakt." << endl;
}

void delete_tree(Treenode *root) {
    if (!root) return;
    
    // Post-order traversal using stack for safe deletion (or simple recursion)
    Treenode *current = root;
    stack<Treenode*> s;
    Treenode *last_visited = nullptr;

    while (current || !s.empty()) {
        if (current) {
            s.push(current);
            current = current->left;
        } else {
            Treenode *peek_node = s.top();
            if (peek_node->right && peek_node->right != last_visited) {
                current = peek_node->right;
            } else {
                s.pop();
                last_visited = peek_node;
                delete last_visited; // Deletes the Contact* inside
            }
        }
    }
}

void print_inorder(Treenode *root) {
    if (!root) {
        cout << "Stablo je prazno." << endl;
        return;
    }
    cout << "\n--- Ispis Inorder (Sorted) ---" << endl;
    Treenode *next = root;
    stack<Treenode*> s;
    while (next || !s.empty()) {
        while (next) {
            s.push(next);
            next = next->left;
        }
        if(!s.empty()) {
            next = s.top();
            s.pop();
            print_node(next);
            next = next->right;
        }
    }
    cout << "-------------------------------" << endl;
}

void print_levelorder(Treenode *root) {
    if (!root) {
        cout << "Stablo je prazno." << endl;
        return;
    }
    cout << "\n--- Ispis Level-Order ---" << endl;
    queue<Treenode*> q;
    Treenode *next = root;
    q.push(next);
    int cnt = 1;
    while (!q.empty()) {
        next = q.front();
        next->id = cnt++;
        q.pop();
        if(next == root) {
            cout <<"node id: " << next->id << ", prezime: " << next->c->surname
            << ", ime: " << next->c->name
                << ",  broj: " << next->c->number << ", ovo je koren" << endl;
        }else {
            cout << "node id: " << next->id << ", prezime: " << next->c->surname
            << ", ime: " << next->c->name
                << ",  broj: " << next->c->number << ", id oca: " << next->parent->id << endl;
        }
        if(next->left) {
            q.push(next->left);
        }
        if(next->right) {
            q.push(next->right);
        }
    }
    cout << "-------------------------" << endl;
}

vector<Treenode*> bin_search_by_surname(const string& prezime, Treenode *root) {
    Treenode *next = root;
    Treenode *temp;
    vector<Treenode*> v;
    while (next) {
        if (prezime == next->c->surname) {
            temp = next;
            v.push_back(next);
            
            // Go left (predecessors)
            Treenode *pred = find_pred(next);
            while (pred && pred->c->surname == prezime) {
                v.push_back(pred);
                pred = find_pred(pred);
            }
            
            // Go right (successors)
            Treenode *succ = find_succ(next);
            while (succ && succ->c->surname == prezime) {
                v.push_back(succ);
                succ = find_succ(succ);
            }
            break;
        }
        if (prezime < next->c->surname) {
            next = next->left;
        } else {
            next = next->right;
        }
    }
    
    if(v.empty()) {
        cout << "Pretraga po prezimenu neuspesna" << endl;
    }
    return v;
}

vector<Treenode*> bin_by_name_and_surname(const string& ime, const string& prezime, Treenode *root) {
    vector<Treenode*> same_surname = bin_search_by_surname(prezime, root);
    if(same_surname.empty()) {
        return same_surname;
    }
    vector<Treenode*> same_name_surname;
    for(size_t i = 0; i < same_surname.size(); i++) {
        if(same_surname[i]->c->name == ime) {
            same_name_surname.push_back(same_surname[i]);
        }
    }
    if (same_name_surname.empty()) {
         cout << "Pretraga po imenu i prezimenu neuspesna" << endl;
    }
    return same_name_surname;
}

void change_details(const string& ime1, const string& prezime1, Treenode *&root) {
    int choice, pos = 0;
    long old_number;
    string new_surname, new_name;
    long new_number;
    
    vector<Treenode*> nodes = bin_by_name_and_surname(ime1, prezime1, root);
    if (nodes.empty()) {
        cout << "Ne mozete promeniti nepostojeci kontakt" << endl;
        return;
    }
    
    print_vector(nodes);
    if (nodes.size() >= 2) {
        cout << "\nKoji kontakt zelite da promenite? Unesite redni broj (0 - " << nodes.size() - 1 << "): ";
        cin >> pos;
        if (pos < 0 || pos >= nodes.size()) {
            cout << "Neispravan odabir." << endl;
            return;
        }
    }
    
    Treenode *my_node = nodes[pos];
    old_number = my_node->c->number;
    
    // Copy existing data
    new_surname = my_node->c->surname;
    new_name = my_node->c->name;
    new_number = my_node->c->number;

    // Delete the old node from the tree (crucial step)
    delete_node(root, prezime1, ime1); 
    
    cout << "Sta hocete da promenite?" << endl << " 1)prezime  2)ime   3)broj" <<endl;
    cin >> choice;

    switch (choice) {
        case 1:
            cout << "Unesite novo prezime: ";
            cin >> new_surname;
            break;
        case 2:
            cout << "Unesite novo ime: ";
            cin >> new_name;
            break;
        case 3:
            cout << "Unesite novi broj: ";
            cin >> new_number;
            break;
        default:
            cout << "Neispravan odabir. Nema promene." << endl;
            return;
    }
    
    // Create and add the new node
    Contact *novi = make_contact(new_name, new_surname, new_number);
    Treenode *ubacujem = new Treenode(novi);
    
    if (!root) {
        root = ubacujem; // Handle case where the deleted node was the root
    } else {
        add_node(root, ubacujem);
    }
    
    cout << "Uspesna promena" << endl;
}