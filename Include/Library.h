#ifndef LIBRARY_H
#define LIBRARY_H

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include "Resources.h"
#include "Users.h"
using namespace std;

class Library
{
private:
    struct BorrowRecord
    {
        int userID;
        LibraryResource *resource;
        time_t borrowDate;
        time_t dueDate;
        time_t returnDate;
        double fine;
        int durationDays;

        BorrowRecord(int uid, LibraryResource *res, int durationDays);
        void markReturned(User *user);
    };

    string libraryName;
    vector<LibraryResource *> resources;
    vector<User *> users;
    vector<BorrowRecord> borrowHistory;

public:
    Library(string name);

    void addResource(LibraryResource *res);
    void showResources() const;

    void showAvailableResources() const;

    void addUser(User *user);
    void registerUser();
    void changePassword(User *u);
    void showUserProfile(User *u) const;
    User* loginUser();

    bool borrowResource(User *user, LibraryResource *res);
    bool returnResource(User *user, LibraryResource *res);

    void showBorrowHistory() const;
    void searchResources() const;

    string getLibraryName() const;
    vector<User *> &getUsers();
    vector<LibraryResource *> &getResources();
    vector<BorrowRecord> &getBorrowHistory();

    ~Library();
};

#endif
