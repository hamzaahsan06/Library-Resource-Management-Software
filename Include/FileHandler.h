#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "Library.h"
#include "Admin.h"
using namespace std;

// functions read data from files 
void ReadUsersFromFile(const string &filename, vector<User *> &users);
void ReadResourcesFromFile(const string &filename, vector<LibraryResource *> &resources);
void ReadBorrowHistoryFromFile(const string &filename, Library &lib);

// functions write data back to files
void SaveUsersToFile(const string &filename, vector<User *> &users);
void SaveResourcesToFile(const string &filename, vector<LibraryResource *> &resources);
void SaveBorrowHistoryToFile(const string &filename, Library &lib);

// helper functions to generate new unique IDs
int generateNewUserID(const string &filename);     // returns max ID + 1 for new registration
int generateNewResourceID(const string &filename); // returns max ID + 1 for new resource

#endif // FILEHANDLER_H