#ifndef LIBRARY_H
#define LIBRARY_H

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include "Resources.h"
#include "Users.h"
using namespace std;

// manages all library operations — resources, users, and borrowing
class Library
{
private:

    // tracks a single borrowing transaction
    struct BorrowRecord
    {
        int userID;                // who borrowed
        LibraryResource *resource; // what was borrowed
        time_t borrowDate;
        time_t dueDate;
        time_t returnDate;         // 0 if not yet returned
        double fine;

        // calculates due date from user's borrow duration
        BorrowRecord(int uid, LibraryResource *res, int durationDays);

        // marks returned and deducts fine if overdue
        void markReturned(User *user);
    };

    string libraryName;
    vector<LibraryResource *> resources; // all resources
    vector<User *> users;                // all registered users
    vector<BorrowRecord> borrowHistory;  // full borrow history

public:
    // vectors start empty, FileHandler fills them from CSV
    Library(string name);

    // ---------- Resource Management ----------
    void addResource(LibraryResource *res);
    void showResources() const;

    // ---------- show only available resources ----------
    void showAvailableResources() const;

    // ---------- User Management ----------
    void addUser(User *user);

    // ---------- create user account from input ----------
    void registerUser();

    // ---------- Borrowing Logic ----------
    bool borrowResource(User *user, LibraryResource *res); // duration comes from user type

    // ---------- Returning Logic ----------
    bool returnResource(User *user, LibraryResource *res);

    // ---------- Borrow History ----------
    void showBorrowHistory() const;

    // ---------- search resources ----------
    vector<LibraryResource *> searchResources(const string &keyword) const;

    // ---------- Getters ----------
    string getLibraryName() const;
    vector<User *> &getUsers();                    // by reference — FileHandler fills directly
    vector<LibraryResource *> &getResources();     // by reference — FileHandler fills directly
    vector<BorrowRecord> &getBorrowHistory();      // by reference — FileHandler fills directly

    // ---------- Destructor ----------
    ~Library(); // frees all heap memory
};

#endif
