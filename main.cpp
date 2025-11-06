#include "Contact.h"
#include "FileIO.h"
#include "BST.h"
#include "LinkedListHistory.h"
#include "ShareHistory.h"
#include <iostream>
#include <vector>
#include <limits> // Required for numeric_limits

using namespace std;

// Forward declaration for memory cleanup
void cleanup_memory(Treenode *root, ListHeader *history, vector<Contact*>& share_history_ptrs) {
    if (root) delete_tree(root);

    // Note: The ListHeader nodes point to Contact* owned by the BST. 
    // Only the Listnode structs themselves should be deleted.
    if (history) {
        Listnode *curr = history->head;
        Listnode *next;
        while (curr) {
            next = curr->next;
            // IMPORTANT: DO NOT delete curr->c (Contact*), as it is owned by the BST
            delete curr;
            curr = next;
        }
        delete history;
    }
    
    // The share_history_ptrs vector holds pointers to Contact* owned by the BST.
    // We only clear the vector, not delete the contacts.
    share_history_ptrs.clear();
}


int main() {
    int n, neaktivnost = 3; // Default inactivity limit set to 3
    vector<Contact*> kontakti; // Share history vector (pointers to BST contacts)
    Treenode *root = nullptr;
    ListHeader *istorija_komunikacije = new_list_header();
    istorija_komunikacije->max_size = 5; // Default communication history size

    // Initial load attempt
    vector<Contact*> initial_contacts = read_file("Contacts20.txt");
    if (!initial_contacts.empty()) {
         root = make_tree(initial_contacts);
         cout << "Inicijalno ucitano " << initial_contacts.size() << " kontakata iz Contacts20.txt." << endl;
    } else {
        cout << "Contacts20.txt nije pronadjen ili je prazan. Stablo kontakata je prazno." << endl;
    }
    
    vector<Treenode*> cont;

    while (true) {
        int choice;
        cout << "\n**************************************"<< endl;
        cout << "Odaberite akciju:"<< endl;
        cout << "1)Ucitaj imenik i kreiraj stablo" << endl;
        cout << "2)Pretraga po imenu ili imenu i prezimenu" << endl;
        cout << "3)Izmena prezimena, imena ili broja"<< endl;
        cout << "4)Dodavanje novog kontakta"<< endl;
        cout << "5)Uklanjanje kontakta sa datim imenom i prezimenom"<< endl;
        cout << "6)Ispis stabla"<< endl;
        cout << "7)Brisanje stabla"<< endl;
        cout << "8)Ogranici istoriju komunikacije"<< endl;
        cout << "9)Stampaj istoriju komunikacije"<< endl;
        cout << "10)Komuniciraj "<< endl;
        cout << "11)Deli sadrzaj "<< endl;
        cout << "12)Stampaj istoriju deljenja "<< endl;
        cout << "13)Ogranici istoriju deljenja (Trenutno max. neaktivnost: " << neaktivnost << ")"<< endl;
        cout << "14)Kraj programa "<< endl;
        cout << "**************************************"<< endl;
        cout << "Vas izbor: ";

        if (!(cin >> choice)) {
            cout << "Neispravan unos. Pokusajte ponovo." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        string ime, prezime;
        long broj;
        int x;
        int pos = 0; // For selection index

        switch (choice) {
            case 1:
                if (root) delete_tree(root); // Clean up old memory
                initial_contacts = read_file("Contacts20.txt");
                root = make_tree(initial_contacts);
                if (root) cout << "Uspresno kreirano stablo sa " << initial_contacts.size() << " kontakata." << endl;
                break;
            case 2:
                cout << "Pretraga 1) samo po prezimenu ili 2) po prezimenu i imenu: ";
                cin >> x;
                cout << "Unesite prezime: "; cin >> prezime;
                if(x == 1) {
                    print_vector(bin_search_by_surname(prezime, root));
                } else if (x == 2) {
                    cout << "Unesite ime: "; cin >> ime;
                    print_vector(bin_by_name_and_surname(ime, prezime, root));
                } else {
                    cout << "Nepravilan odabir" << endl;
                }
                break;
            case 3:
                cout << "Unesite prezime kontakta za izmenu: "; cin >> prezime;
                cout << "Unesite ime kontakta za izmenu: "; cin >> ime;
                change_details(ime, prezime, root);
                break;
            case 4: {
                Contact *novi = new Contact;
                cout << "Unesite prezime: "; cin >> novi->surname;
                cout << "Unesite ime: "; cin >> novi->name;
                cout << "Unesite broj: "; cin >> novi->number;
                novi->x = 0; // Initialize share inactivity
                Treenode *temp = new Treenode(novi);
                if (!root) {
                    root = temp;
                } else {
                    add_node(root, temp);
                }
                cout << "Uspesno dodat kontakt" << endl;
                break;
            }
            case 5:
                cout << "Unesite prezime: "; cin >> prezime;
                cout << "Unesite ime: "; cin >> ime;
                delete_node(root, prezime, ime);
                break;
            case 6:
                print_inorder(root);
                print_levelorder(root);
                break;
            case 7:
                if(root) delete_tree(root);
                root = nullptr;
                cout << "Stablo je obrisano." << endl;
                break;
            case 8:
                cout << "Unesite max broj kontakata za istoriju komunikacije: ";
                cin >> n;
                istorija_komunikacije->max_size = n;
                cout << "Maksimalna velicina istorije komunikacije postavljena na " << n << endl;
                break;
            case 9:
                print_history(istorija_komunikacije);
                break;
            case 10:
                cout << "Unesite prezime: "; cin >> prezime;
                cout << "Unesite ime: "; cin >> ime;
                cont = bin_by_name_and_surname(ime, prezime, root);
                pos = 0;
                if(cont.empty()) break;
                else if(cont.size() >= 2){
                    print_vector(cont);
                    cout << "\nSa kojim kontaktom zelite da komunicirate? Unesite redni broj (0 - " << cont.size() - 1 << "): ";
                    cin >> pos;
                }
                if (pos >= 0 && pos < cont.size()) {
                    komuniciraj(istorija_komunikacije, cont[pos]->c);
                    cout << "Uspesno zabelezena komunikacija." << endl;
                }
                break;
            case 11:
                cout << "Unesite prezime: "; cin >> prezime;
                cout << "Unesite ime: "; cin >> ime;
                cont = bin_by_name_and_surname(ime, prezime, root);
                pos = 0;
                if(cont.empty()) break;
                else if(cont.size() >= 2){
                    print_vector(cont);
                    cout << "\nSa kojim kontaktom zelite da delite sadrzaj? Unesite redni broj (0 - " << cont.size() - 1 << "): ";
                    cin >> pos;
                }
                if (pos >= 0 && pos < cont.size()) {
                    kontakti = share_with_contact(kontakti, cont[pos]->c, neaktivnost);
                    cout << "Uspesno zabelezeno deljenje." << endl;
                }
                break;
            case 12:
                print_share_history(kontakti);
                break;
            case 13:
                cout << "Unesite max neaktivnost (h): ";
                cin >> neaktivnost;
                cout << "Maksimalna neaktivnost za istoriju deljenja postavljena na " << neaktivnost << endl;
                break;
            case 14:
                cleanup_memory(root, istorija_komunikacije, kontakti);
                cout << "Kraj programa. Memorija oslobodjena." << endl;
                return 0;
            default:
                cout << "Nepravilan odabir." << endl;
        }
    }
}