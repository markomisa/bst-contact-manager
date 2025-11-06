#include "ShareHistory.h"
#include <iostream>
#include <algorithm>

using namespace std;

// Helper: Finds contact index in the sharing history vector
static int find_in_history1(const vector<Contact*>& contacts, Contact* person) {
    for (size_t i = 0; i < contacts.size(); i++) {
        if (contacts[i]->name == person->name && contacts[i]->surname == person->surname && contacts[i]->number == person->number) {
            return i;
        }
    }
    return -1;
}

/**
 * @brief Simulates content sharing, promoting the contact and handling inactivity (x) limit (h).
 * @param contacts The vector representing share history.
 * @param person The contact shared with.
 * @param h The maximum inactivity threshold.
 * @return The updated share history vector.
 */
vector<Contact*> share_with_contact(vector<Contact*> contacts, Contact* person, int h) {
    int ind = find_in_history1(contacts, person);
    
    if (ind == -1) {
        // Contact not in history, add to end (new lowest priority)
        contacts.push_back(person);
        contacts.back()->x = 0;
    } else {
        // Contact found, reset inactivity and move one position up (if not already at 0)
        contacts[ind]->x = 0;
        if (ind != 0) {
            // Swap with the element before it
            Contact *temp = contacts[ind];
            contacts[ind] = contacts[ind - 1];
            contacts[ind - 1] = temp;
            contacts[ind - 1]->x = 0; // Reset new position's inactivity (already done above, but harmless)
        }
    }

    // Increment inactivity (x) for all *other* contacts
    for (size_t i = 0; i < contacts.size(); i++) {
        if (contacts[i] != person) { // Comparing pointers is sufficient if 'person' comes from BST
            contacts[i]->x++;
        }
    }

    // Move contacts that reached the inactivity threshold (h) to the back
    for (int i = 0; i < (int)contacts.size(); i++) {
        if (contacts[i]->x >= h && h > 0) { // Only check if h is active
            Contact* temp = contacts[i];

            // Shift elements left to fill the gap
            for (size_t j = i; j < contacts.size() - 1; j++) {
                contacts[j] = contacts[j + 1];
            }
            
            // Move the inactive contact to the back
            contacts.back() = temp;
            contacts.back()->x = 0; // Reset its inactivity (or leave it high if we want it to stay at the back)
            i--; // Re-check the current index since a new element was shifted into it
        }
    }
    return contacts;
}

void print_share_history(const vector<Contact*>& contacts) {
    if(contacts.empty()) {
        cout << "Nema istorije deljenja." << endl;
        return;
    }
    cout << "\n--- Istorija Deljenja (Prioritet Opadajuci) ---" << endl;
    for (size_t i = 0; i < contacts.size(); i++) {
        cout << i+1 << ": " <<contacts[i]->name << " " << contacts[i]->surname << " "
             << contacts[i]->number<< " [Neaktivnost: " << contacts[i]->x << "]" << endl;
    }
    cout << "-----------------------------------------------" << endl;
}