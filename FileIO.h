#ifndef FILE_IO_H
#define FILE_IO_H

#include "Contact.h"
#include <vector>
#include <string>

std::vector<Contact*> read_file(const std::string& filename);

#endif // FILE_IO_H