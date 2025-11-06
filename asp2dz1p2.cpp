#include <string>
#include <vector>
#include <iostream>
#include <stack>
#include <tuple>
#include <fstream>
#include <sstream>
#include <queue>
using namespace std;

typedef struct Contact {
    string name;
    string surname;
    long number;
    int x;
}Contact;

typedef struct Listnode {
    struct Listnode *next;
    Contact *c;
}Listnode;

typedef struct ListHeader {
    Listnode *head;
    int curr_size, max_size;
}ListHeader;

Listnode* find_in_history(ListHeader* history, Contact* c) {
    Listnode* prev = nullptr;
    Listnode* current = history->head;

    while (current != nullptr) {
        if (current->c->name == c->name && current->c->surname == c->surname) {
            return prev;
        }
        prev = current;
        current = current->next;
    }
    return nullptr;
}

void move_to_front(ListHeader* history, Listnode* prev, Listnode* current) {
    if (prev != nullptr) {
        prev->next = current->next;
        current->next = history->head;
        history->head = current;
    }
}

void komuniciraj(ListHeader* history, Contact* c) {
    Listnode* prev = find_in_history(history, c);
    if (prev != nullptr || (history->head != nullptr && history->head->c->name == c->name && history->head->c->surname == c->surname  &&
        history->head->c->number == c->number)) {
        Listnode* current = (prev == nullptr) ? history->head : prev->next;
        move_to_front(history, prev, current);
        return;
    }

    if (history->curr_size == history->max_size) {
        Listnode* temp = history->head;
        Listnode* prev_tail = nullptr;
        while (temp->next != nullptr) {
            prev_tail = temp;
            temp = temp->next;
        }
        delete temp;
        if (prev_tail != nullptr) {
            prev_tail->next = nullptr;
        } else {
            history->head = nullptr;
        }
        history->curr_size--;
    }

    Listnode* new_node = new Listnode;
    new_node->c = c;
    new_node->next = history->head;
    history->head = new_node;
    history->curr_size++;
}

void delete_listnode(ListHeader *l) {
    Listnode *temp, *curr = l->head;
    while (curr->next->next) {
        curr = curr->next;
    }
    temp = curr->next;
    curr->next = nullptr;
    delete(temp);
}

void add_listnode_front(ListHeader *l, Contact *contact) {
    Listnode *new_node = new Listnode;
    new_node->c = contact;
    if(!l->head) {
        l->head = new_node;
        l->head->next = nullptr;
        l->curr_size = 1;
    }else {
        new_node->next = l->head;
        l->head = new_node;
        int size = ++l->curr_size;//mozda lose prebrojim
        if(size > l->max_size) {
            delete_listnode(l);
        }
    }
    if(l->curr_size > l->max_size) {
        l->curr_size = l->max_size;
    }
}

ListHeader *new_list_header() {
    ListHeader *l = new ListHeader;
    l->head = nullptr;
    l->curr_size = 0;
    return l;
}

void print_listnode(Listnode *node){
    cout << "prezime: " << node->c->surname
    << ", ime: " << node->c->name
    << ", telefonski broj: " << node->c->number << endl;
}

void print_history(ListHeader *l) {
    if(!l->head) {
        cout << "Trenutno nema istorije" << endl;
    }
    else {
        cout << endl;
        cout << "Trenutna istorija" << endl;
        Listnode *curr = l->head;
        while (curr) {
            print_listnode(curr);
            curr = curr->next;
        }
    }
    cout << endl;
}

void delete_node_if_in_history(ListHeader *l, Contact *c) {
    if(!l->head) return;
    Listnode *curr = l->head;
    if((curr->c->name==c->name) && (curr->c->surname==c->surname) && (curr->c->number==c->number)) {
        if(l->curr_size == 1) {
            l->head = nullptr;
            return;
        }
        l->head = l->head->next;
    }
    while(curr->next) {
        if((curr->next->c->name==c->name) && (curr->next->c->surname==c->surname) && (curr->next->c->number==c->number)) {
            curr->next = curr->next->next;
            l->curr_size--;
            return;
        }
        curr = curr->next;
    }
}

int find_in_history1(vector<Contact*> contacts, Contact* person) {
    for (int i = 0; i < contacts.size(); i++) {
        if (contacts[i]->name == person->name && contacts[i]->surname == person->surname && contacts[i]->number == person->number) {
            return i;
        }
    }
    return -1;
}

vector<Contact*> share_with_contact(vector<Contact*> contacts, Contact* person, int h) {
    int ind = find_in_history1(contacts, person);
    if(ind == -1) {
        contacts.push_back(person);
        contacts[contacts.size() - 1]->x = 0;
    }else {
        contacts[ind]->x = 0;
        if(ind != 0) {
            Contact *temp = contacts[ind];
            contacts[ind] = contacts[ind - 1];
            contacts[ind - 1] = temp;
            contacts[ind - 1]->x = 0;
        }
    }
    for (int i = 0; i < contacts.size(); i++) {
        if(contacts[i]->number != person->number || contacts[i]->name != person->name || contacts[i]->surname != person->surname) {
            contacts[i]->x++;
        }
    }
    for (int i = 0; i < contacts.size(); i++) {
        if(contacts[i]->x == h) {
            Contact* temp = contacts[i];

            for (int j = i; j < contacts.size() - 1; j++) {
                contacts[j] = contacts[j + 1];
            }
            contacts[contacts.size() - 1] = temp;
            contacts[contacts.size() - 1]->x = 0;
            i--;
        }
    }
    return contacts;
}

void print_share_history(vector<Contact*> contacts) {
    if(contacts.size() == 0) cout << "Nema istorije " << endl;
    for (int i = 0; i < contacts.size(); i++) {
        cout << i+1 << ": " <<contacts[i]->name << " " << contacts[i]->surname << " "
             << contacts[i]->number<< " " << contacts[i]->x << endl;
    }
}

class Treenode {
public:
    Treenode(Contact *c1) {
        c = c1;
        left = right = parent = nullptr;
    }
    ~Treenode() {
        delete c;
    }
    Contact* c;
    Treenode *parent;
    Treenode *left;
    Treenode *right;
    int id;
};

vector<Contact*> read_file(string filename) {
    vector<Contact*> contacts;
    ifstream input_file(filename);
    string line;
    getline(input_file, line);

    while(getline(input_file, line)) {
        istringstream s(line);
        string name, surname;
        long number;
        getline(s, surname, ',');
        getline(s, name, ',');
        s >> number;

        Contact *c = new Contact();
        c->name = name;
        c->surname = surname;
        c->number = number;
        contacts.push_back(c);
    }
    input_file.close();
    return contacts;
}

Treenode *make_tree(vector<Contact*> contacts){
    Treenode *parent_temp;
    stack <tuple<Treenode*, int, int> > st;
    int n = contacts.size();
    int low = 0, high = n - 1, mid = (low + high) / 2;
    Treenode *root = new Treenode(contacts[mid]);
    root->parent = nullptr;
    Treenode *next = root;
    st.push(make_tuple(next, mid + 1, high));
    high = mid - 1;
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
    next->left = nullptr;
    while (!(st.empty()))
    {
        tuple<Treenode*, int, int> t = st.top();
        st.pop();
        next = get<0>(t), low = get<1>(t), high = get<2>(t);
        if (low <= high)
        {
            parent_temp = next;
            mid = (low + high)/2;
            next->right = new Treenode(contacts[mid]);
            next = next->right;
            next->parent = parent_temp;
            st.push(make_tuple(next, mid + 1, high));
            high = mid - 1;
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
            next->left = nullptr;
        }
        else next->right = nullptr;
    }
    return root;
}

Treenode *bst_min(Treenode *root) {
    Treenode *p = root;
    while (p->left != nullptr) {
        p = p->left;
    }
    return p;
}

Treenode *bst_max(Treenode *root) {
    Treenode *p = root;
    while (p->right != nullptr) {
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

void add_node(Treenode *root, Treenode *new_node) {
    Treenode *p = root, *q = nullptr;
    while (p != nullptr) {
        q = p;
        if (new_node->c->surname < p->c->surname) {
            p = p->left;
        } else if (new_node->c->surname > p->c->surname) {
            p = p->right;
        } else {
            if (new_node->c->name < p->c->name) {
                p = p->left;
            } else {
                p = p->right;
            }
        }
    }
    if (new_node->c->surname < q->c->surname) {
        q->left = new_node;
        new_node->parent = q;
    } else if (new_node->c->surname > q->c->surname) {
        q->right = new_node;
        new_node->parent = q;
    } else {
        if (new_node->c->name < q->c->name) {
            q->left = new_node;
            new_node->parent = q;
        } else {
            q->right = new_node;
            new_node->parent = q;
        }
    }
}

vector<Treenode*> bin_search_by_surname(string prezime, Treenode *root) {
    Treenode *next = root;
    Treenode *temp;
    vector<Treenode*> v;
    while (next) {
        if (prezime == next->c->surname) {
            temp = next;
            v.push_back(next);
            Treenode *pred = find_pred(next);
            while (pred && pred->c->surname == prezime) {
                v.push_back(pred);
                pred = find_pred(pred);
            }
            next = temp;
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
    if(v.size() == 0) {
        cout << "Pretraga neuspesna" << endl;
    }
    return v;
}

vector<Treenode*> bin_by_name_and_surname(string ime, string prezime, Treenode *root) {
    vector<Treenode*> same_surname = bin_search_by_surname(prezime, root);
    if(same_surname.size() == 0) {
        cout << "Neuspesno" << endl;
        return same_surname;
    }
    vector<Treenode*> same_name_surname;
    for(int i = 0; i < same_surname.size(); i++) {
        if(same_surname[i]->c->name == ime) {
            same_name_surname.push_back(same_surname[i]);
        }
    }
    return same_name_surname;
}

void print_node(Treenode *node){
    cout << "prezime: " << node->c->surname
    << ", ime: " << node->c->name
    << ", telefonski broj: " << node->c->number << endl;
}

void print_levelorder(Treenode *root) {
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
    cout << endl;
}

Contact *make_contact(string name, string surname, long number) {
    Contact *c = new Contact;
    c->name = name;
    c->surname = surname;
    c->number = number;
    return c;
}

void print_vector(vector<Treenode*> nodes) {
    cout << endl;

    for(int i = 0; i < nodes.size(); i++) {
        print_node(nodes[i]);
    }
    cout << endl;
}

void delete_node(Treenode *root, string surname, string name) {
    int pos = 0;
    vector<Treenode*> candidates = bin_by_name_and_surname(name, surname, root);
    if (candidates.empty()) {
        cout << "Ne mozete obrisati nepostojeci kontakt" << endl;
        return;
    }
    print_vector(candidates);
    if (candidates.size() >= 2) {
        cout << endl;
        cout << "Koji kontakt zelite da obrisete? Unesite redni broj 0 - " << candidates.size() - 1 << endl;
        cin >> pos;
    }
    Treenode *brisem = candidates[pos];
    Treenode *temp;

    if (brisem->left == nullptr && brisem->right == nullptr) {
        if (brisem->parent) {
            if (brisem->parent->left == brisem) {
                brisem->parent->left = nullptr;
            } else {
                brisem->parent->right = nullptr;
            }
        } else {
            root = nullptr;
        }
        delete brisem;
    }
    else if (brisem->left != nullptr && brisem->right != nullptr) {
        temp = find_pred(brisem);

        if (temp->parent->left == temp) {
            temp->parent->left = temp->left;
        } else {
            temp->parent->right = temp->left;
        }

        temp->left = brisem->left;
        if (brisem->right != temp) {
            temp->right = brisem->right;
        }

        if (brisem->parent) {
            if (brisem->parent->left == brisem) {
                brisem->parent->left = temp;
            } else {
                brisem->parent->right = temp;
            }
        } else {
            root = temp;
        }

        temp->parent = brisem->parent;
        delete brisem;
    }
    else {
        if (brisem->left) {
            temp = brisem->left;
        } else {
            temp = brisem->right;
        }
        if (brisem->parent) {
            if (brisem->parent->left == brisem) {
                brisem->parent->left = temp;
            } else {
                brisem->parent->right = temp;
            }
        } else {
            root = temp;
        }

        temp->parent = brisem->parent;
        delete brisem;
    }
}

void print_inorder(Treenode *root) {
    Treenode *next = root;
    stack<Treenode*> s;
    while (1) {
        while (next) {
            s.push(next);
            next = next->left;
        }
        if(!s.empty()) {
            next = s.top();
            s.pop();
            print_node(next);
            next = next->right;
        }else return;
    }
}

void delete_tree(Treenode *root) {
    Treenode *next = root;
    stack<Treenode*> s;
    while (1) {
        while (next) {
            s.push(next);
            next = next->left;
        }
        if(!s.empty()) {
            next = s.top();
            s.pop();
            Treenode *old = next;
            next = next->right;
            delete(old);
        }else return;
    }
}

void change_details(string ime1, string prezime1, Treenode *root) {
    int choice, pos = 0;
    string prezime, ime;
    long broj;
    vector<Treenode*> nodes = bin_by_name_and_surname(ime1, prezime1, root);
    if (nodes.empty()) {
        cout << "Ne mozete promeniti nepostojeci kontakt" << endl;
        return;
    }
    cout << endl;
    print_vector(nodes);
    if (nodes.size() >= 2) {
        cout << endl;
        cout << "Koji kontakt zelite da promenite? Unesite redni broj 0 - " << nodes.size() - 1 << endl;
        cin >> pos;
    }
    Treenode *my_node = nodes[pos];
    long old_number = my_node->c->number;
    delete_node(root, prezime1, ime1);
    cout << "Sta hocete da promenite" << endl <<
        " 1)prezime  2)ime   3)broj" <<endl;
    cin >> choice;
    Treenode *ubacujem;
    Contact *novi;
    switch (choice) {
        case 1:
            cout << "Unesite novo prezime" <<endl;
            novi = make_contact(ime1, prezime1, old_number);
            cin >> novi->surname;
            ubacujem = new Treenode(novi);
            add_node(root,ubacujem);
        break;
        case 2:
            cout << "Unesite novo ime" <<endl;
            novi = make_contact(ime1, prezime1, old_number);
            cin >> novi->name;
            ubacujem = new Treenode(novi);
            add_node(root,ubacujem);
        break;
        case 3:
            cout << "Unesite novi broj" <<endl;
            novi = make_contact(ime1, prezime1, old_number);
            cin >> novi->number;
            ubacujem = new Treenode(novi);
            add_node(root,ubacujem);
        break;
    }
    cout << "Uspesna promena" << endl;
}

int main() {
    int n, size = 0, neaktivnost;
    vector<Contact*> kontakti;
    Treenode *root = make_tree(read_file("Contacts20.txt")),  *temp;
    ListHeader *istorija_komunikacije = new_list_header();
    vector<Treenode*> cont;
    while (1) {
        int choice;
        cout << endl;
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
        cout << "13)Ogranici istoriju deljenja"<< endl;
        cout << "14)Kraj programa "<< endl;
        cin >> choice;
        Contact *novi = new Contact;
        string ime, prezime;
        long broj;
        int x;
        switch (choice) {
            case 1:
                root = make_tree(read_file("Contacts20.txt"));
                cout << "Uspresno kreirano stablo" << endl;
                break;
            case 2:
                cout << "Pretraga 1) samo po prezimenu ili 2) po prezimenu i imenu" << endl;
                cin >> x;
                if(x == 1) {
                    cout << "Unesite prezime: ";
                    cin >> prezime;
                    print_vector(bin_search_by_surname(prezime, root));
                }else if(x == 2) {
                    cout << "Unesite prezime: ";
                    cin >> prezime;
                    cout << "Unesite ime: ";
                    cin >> ime;
                    print_vector(bin_by_name_and_surname(ime, prezime, root));
                }else {
                    cout << "Nepravilan odabir" << endl;
                    break;
                }
                break;
            case 3:
                cout << "Unesite prezime: ";
                cin >> prezime;
                cout << "Unesite ime: ";
                cin >> ime;
                change_details(ime, prezime, root);
                break;
            case 4:
                cout << "Unesite prezime: ";
                cin >> novi->surname;
                cout << "Unesite ime: ";
                cin >> novi->name;
                cout << "Unesite broj: ";
                cin >> novi->number;
                temp = new Treenode(novi);
                add_node(root, temp);
                cout << "Uspesno dodat kontakt" << endl;
                break;
            case 5:
                cout << "Unesite prezime: ";
                cin >> prezime;
                cout << "Unesite ime: ";
                cin >> ime;
                delete_node(root, prezime, ime);
                break;
            case 6:
                print_inorder(root);
                cout << endl;
                print_levelorder(root);
                break;
            case 7:
                if(root) delete_tree(root);
                break;
            case 8:
                cout << "Unesite max broj kontakata: ";
                cin >> n;
                istorija_komunikacije->max_size = n;
                break;
            case 9:
                print_history(istorija_komunikacije);
                break;
            case 10:
                print_history(istorija_komunikacije);
                cout << "Unesite prezime: ";
                cin >> prezime;
                cout << "Unesite ime: ";
                cin >> ime;
                cont = bin_by_name_and_surname(ime, prezime, root);
                n = 0;
                if(cont.empty()) break;
                else if(cont.size() >= 2){
                    print_vector(cont);
                    cout << endl;
                    cout << "Sa kojim kontaktom zelite da komunicirate? Unesite redni broj 0 - " << cont.size() - 1 << endl;
                    cin >> n;
                }
                komuniciraj(istorija_komunikacije, cont[n]->c);
                cout << "Uspesno";
                break;
            case 11:
                cout << "Unesite prezime: ";
                cin >> prezime;
                cout << "Unesite ime: ";
                cin >> ime;
                cont = bin_by_name_and_surname(ime, prezime, root);
                print_vector(cont);
                n = 0;
                if(cont.empty()) break;
                else if(cont.size() >= 2){
                    print_vector(cont);
                    cout << endl;
                    cout << "Sa kojim kontaktom zelite da delite sadrzaj? Unesite redni broj 0 - " << cont.size() - 1 << endl;
                    cin >> n;
                }
                kontakti = share_with_contact(kontakti, cont[n]->c, neaktivnost);
                break;
            case 12:
                print_share_history(kontakti);
                break;
            case 13:
                cout << "Unesite max neaktivnost: ";
                cin >> neaktivnost;
                break;
            case 14:
                return 0;
        }
    }
}

