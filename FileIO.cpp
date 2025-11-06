#include "FileIO.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

/**
 * @brief Reads contact data from a CSV file.
 * @param filename The path to the file.
 * @return A vector of pointers to newly allocated Contact objects.
 */
vector<Contact*> read_file(const string& filename) {
    vector<Contact*> contacts;
    ifstream input_file(filename);
    string line;
    
    if (!input_file.is_open()) {
        cerr << "Greska pri otvaranju datoteke: " << filename << endl;
        return contacts;
    }

    // Read and discard header line
    getline(input_file, line); 

    while(getline(input_file, line)) {
        istringstream s(line);
        string name, surname_str, number_str;
        long number;
        
        // Extract surname
        getline(s, surname_str, ',');
        
        // Extract name
        getline(s, name, ',');
        
        // Extract number (must handle the rest of the stream, assuming number is last)
        if (!(s >> number)) {
            // Basic error handling for number conversion
            continue; 
        }

        Contact *c = new Contact();
        c->name = name;
        c->surname = surname_str;
        c->number = number;
        c->x = 0; // Initialize share inactivity
        contacts.push_back(c);
    }
    input_file.close();
    return contacts;
}