#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "Library.h"
#include "admin.h"
using namespace std;

// ---------- Load Functions ----------
void ReadUsersFromFile(const string &filename, vector<User *> &users);
void ReadResourcesFromFile(const string &filename, vector<LibraryResource *> &resources);

// ---------- Save Functions ----------
void SaveUsersToFile(const string &filename, vector<User *> &users);
void SaveResourcesToFile(const string &filename, vector<LibraryResource *> &resources);

// ---------- ID Generation ----------
int generateNewUserID(const string &filename);     // returns max ID + 1 for new registration
int generateNewResourceID(const string &filename); // returns max ID + 1 for new resource

#endif // FILEHANDLER_H