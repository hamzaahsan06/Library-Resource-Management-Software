#ifndef LIBRARY_H
#define LIBRARY_H

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include "Resources.h"
#include "Users.h"
using namespace std;

// --------------------- Borrow Record ---------------------
struct BorrowRecord
{
    LibraryResource* resource; // Pointer to borrowed resource
    time_t borrowDate;
    time_t dueDate;
    time_t returnDate;
    double fine;

    BorrowRecord(LibraryResource* res, int durationDays)
        : resource(res), fine(0.0), returnDate(0)
    {
        borrowDate = time(0);
        dueDate = borrowDate + durationDays * 24 * 60 * 60;
    }

    // Mark resource as returned and calculate fine if overdue
    void markReturned(User* user)
    {
        returnDate = time(0);
        if (difftime(returnDate, dueDate) > 0)
        {
            int daysLate = static_cast<int>(difftime(returnDate, dueDate) / (60 * 60 * 24));
            fine = daysLate * user->getFineRate();
            user->updateBalance(-fine);
        }
    }
};

// --------------------- Library Class ---------------------
class Library
{
private:
    string libraryName;
    vector<LibraryResource*> resources;          // All resources
    vector<User*> users;                         // All users
    vector<BorrowRecord> borrowHistory;          // Tracks all borrowings

public:
    // ---------- Constructor ----------
    Library(string name) : libraryName(name) {}

    // ---------- Resource Management ----------
    void addResource(LibraryResource* res)
    {
        resources.push_back(res);
        cout << "Resource \"" << res->getTitle() << "\" added to " << libraryName << endl;
    }

    void showResources() const
    {
        cout << "\nResources in " << libraryName << ":\n";
        for (auto res : resources)
        {
            res->displayInfo();
            cout << "-----------------------\n";
        }
    }

    // ---------- User Management ----------
    void addUser(User* user)
    {
        users.push_back(user);
        cout << "User \"" << user->getFullName() << "\" added to " << libraryName << endl;
    }

    void showUsers() const
    {
        cout << "\nUsers of " << libraryName << ":\n";
        for (auto user : users)
        {
            user->displayInfo();
            cout << "-----------------------\n";
        }
    }

    // ---------- Borrowing Logic ----------
    bool borrowResource(User* user, LibraryResource* res, int durationDays = 10)
    {
        // Daily limit enforcement
        int dailyLimit = user->getDailyLimit();
        int countToday = 0;

        time_t now = time(0);
        for (auto& record : borrowHistory)
        {
            if (record.resource && record.borrowDate >= now - 24*60*60 && record.resource)
            {
                // Count only active borrowings of this user today
                if (record.returnDate == 0 && user->getUserID() == record.resource->getResourceID()) 
                    countToday++;
            }
        }

        if (countToday >= dailyLimit)
        {
            cout << user->getFullName() << " has reached the daily borrow limit of " << dailyLimit << " resources.\n";
            return false;
        }

        // Check availability
        if (!res->isAvailable())
        {
            cout << "Resource \"" << res->getTitle() << "\" is not available.\n";
            return false;
        }

        // Borrow resource
        res->borrowResource();
        BorrowRecord record(res, durationDays);
        borrowHistory.push_back(record);

        cout << user->getFullName() << " successfully borrowed \"" << res->getTitle() << "\".\n";
        return true;
    }

    // ---------- Returning Logic ----------
    bool returnResource(User* user, LibraryResource* res)
    {
        for (auto& record : borrowHistory)
        {
            if (record.resource == res && record.returnDate == 0)
            {
                // Mark as returned
                record.markReturned(user);
                res->returnResource();
                cout << user->getFullName() << " returned \"" << res->getTitle() << "\".\n";

                if (record.fine > 0)
                {
                    cout << "Overdue! Fine of " << record.fine << " applied.\n";
                }
                return true;
            }
        }

        cout << user->getFullName() << " has not borrowed \"" << res->getTitle() << "\".\n";
        return false;
    }

    // ---------- Borrow History ----------
    void showBorrowHistory() const
    {
        cout << "\nBorrow History:\n";
        for (auto record : borrowHistory)
        {
            cout << "Resource: " << record.resource->getTitle()
                 << ", Borrowed: " << ctime(&record.borrowDate);
            if (record.returnDate != 0)
                cout << ", Returned: " << ctime(&record.returnDate)
                     << ", Fine: " << record.fine << endl;
            else
                cout << ", Not Returned Yet\n";
        }
    }

    // ---------- Getters ----------
    string getLibraryName() const { return libraryName; }
};

#endif
