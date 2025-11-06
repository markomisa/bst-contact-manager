#ifndef BST_H
#define BST_H

#include "Contact.h"
#include <vector>
#include <string>

// --- Helper Functions Declaration ---
Contact *make_contact(const std::string& name, const std::string& surname, long number);
void print_node(Treenode *node);
void print_vector(const std::vector<Treenode*>& nodes);

// --- Core BST Functions Declaration ---
Treenode *make_tree(std::vector<Contact*>& contacts);
void add_node(Treenode *root, Treenode *new_node);
void delete_node(Treenode *&root, const std::string& surname, const std::string& name);
void delete_tree(Treenode *root);
void change_details(const std::string& name1, const std::string& surname1, Treenode *&root);

// --- Traversal and Search Functions Declaration ---
void print_inorder(Treenode *root);
void print_levelorder(Treenode *root);
std::vector<Treenode*> bin_search_by_surname(const std::string& prezime, Treenode *root);
std::vector<Treenode*> bin_by_name_and_surname(const std::string& ime, const std::string& prezime, Treenode *root);

#endif // BST_H