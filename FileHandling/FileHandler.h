#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "../include/Library.h"
#include "../include/admin.h"
using namespace std;

// ---------- Load Functions ----------
void ReadUsersFromFile(const string &filename, vector<User *> &users);
void ReadResourcesFromFile(const string &filename, vector<LibraryResource *> &resources);
void ReadBorrowHistoryFromFile(const string &filename, Library &lib);

// ---------- Save Functions ----------
void SaveUsersToFile(const string &filename, vector<User *> &users);
void SaveResourcesToFile(const string &filename, vector<LibraryResource *> &resources);
void SaveBorrowHistoryToFile(const string &filename, Library &lib);

// ---------- ID Generation ----------
int generateNewUserID(const string &filename);     // returns max ID + 1 for new registration
int generateNewResourceID(const string &filename); // returns max ID + 1 for new resource

#endif // FILEHANDLER_H