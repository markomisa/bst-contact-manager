#ifndef SHARE_HISTORY_H
#define SHARE_HISTORY_H

#include "Contact.h"
#include <vector>

std::vector<Contact*> share_with_contact(std::vector<Contact*> contacts, Contact* person, int h);
void print_share_history(const std::vector<Contact*>& contacts);

#endif // SHARE_HISTORY_H   
