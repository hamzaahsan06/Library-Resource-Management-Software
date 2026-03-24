#include "Library.h"
#include <iostream>
#include <iomanip>
using namespace std;

// ===================== BorrowRecord =====================

// calculates due date from user's borrow duration
Library::BorrowRecord::BorrowRecord(int uid, LibraryResource *res, int durationDays)
    : userID(uid), resource(res), fine(0.0), returnDate(0)
{
    borrowDate = time(0);
    dueDate = borrowDate + durationDays * 24 * 60 * 60;
}

// marks returned and deducts fine from balance if returned late
void Library::BorrowRecord::markReturned(User *user)
{
    returnDate = time(0);
    if (difftime(returnDate, dueDate) > 0)
    {
        int daysLate = static_cast<int>(difftime(returnDate, dueDate) / (60 * 60 * 24));
        fine = daysLate * user->getFineRate();
        user->updateBalance(-fine);
    }
}

// ===================== Library =====================

// vectors start empty, FileHandler fills them from CSV
Library::Library(string name) : libraryName(name) {}

// frees all heap memory on program exit
Library::~Library()
{
    for (auto user : users)
        delete user;

    for (auto res : resources)
        delete res;
}

// ---------- Resource Management ----------
void Library::addResource(LibraryResource *res)
{
    resources.push_back(res);
    cout << "Resource \"" << res->getTitle() << "\" added to " << libraryName << endl;
}

// ---------- User Management ----------
void Library::addUser(User *user)
{
    users.push_back(user);
    cout << "User \"" << user->getName() << "\" added to " << libraryName << endl;
}

void Library::showResources() const
{
    cout << "\n"
         << "Resources in " << libraryName << endl;
    cout << left << setw(6) << "ID"
         << setw(35) << "Title"
         << setw(15) << "Type"
         << setw(15) << "Available" << endl;
    cout << string(71, '-') << endl;

    for (auto res : resources)
    {
        cout << left << setw(6) << res->getResourceID()
             << setw(35) << res->getTitle()
             << setw(15) << res->getType()
             << setw(15) << res->getAvailableCopies() << endl;
    }
}

// ---------- Borrowing Logic ----------
// duration is taken from user type — student=7, teacher=14, staff=14, premium=30
bool Library::borrowResource(User *user, LibraryResource *res)
{
    int countToday = 0;
    time_t now = time(0); // get current time

    // loop through each borrow record by reference so changes reflect in original
    for (auto &record : borrowHistory)
    {
        if (record.userID == user->getUserID() &&             // same user
            record.returnDate == 0 &&                         // not yet returned
            difftime(now, record.borrowDate) <= 24 * 60 * 60) // borrowed within last 24 hours
        {
            countToday++;
        }
    }

    // block borrow if user has reached their daily limit
    // student=2, teacher=3, staff=4, premium=5
    if (countToday >= user->getDailyLimit())
    {
        cout << user->getName() << " has reached the daily borrow limit of "
             << user->getDailyLimit() << " resources." << endl;
        return false;
    }

    // block borrow if no copies available
    if (!res->isAvailable())
    {
        cout << "Resource \"" << res->getTitle() << "\" is not available." << endl;
        return false;
    }

    // decrease available copies by 1 in resource object
    res->borrowResource();

    // get borrow duration from user type and create borrow record
    borrowHistory.push_back(BorrowRecord(user->getUserID(), res, user->getBorrowDays()));

    cout << user->getName() << " successfully borrowed \"" << res->getTitle() << "\"." << endl;
    return true;
}

// ---------- Returning Logic ----------
bool Library::returnResource(User *user, LibraryResource *res)
{
    // search borrow history for a matching unreturned record
    for (auto &record : borrowHistory) // loop through each borrow record by reference so changes reflect in original
    {
        if (record.userID == user->getUserID() && // same user
            record.resource == res &&             // same resource
            record.returnDate == 0)               // not yet returned
        {
            // set return date and calculate fine if overdue
            // fine is deducted from user balance inside markReturned
            record.markReturned(user);

            // increase available copies by 1 in resource object
            res->returnResource();

            cout << user->getName() << " returned \"" << res->getTitle() << "\"." << endl;

            if (record.fine > 0)
                cout << "Overdue! Fine of " << record.fine << " applied." << endl;

            return true;
        }
    }

    // no active borrow record found for this user and resource
    cout << user->getName() << " has not borrowed \"" << res->getTitle() << "\"." << endl;
    return false;
}

// ---------- Borrow History ----------
void Library::showBorrowHistory() const
{
    cout << "\n"
         << "Borrow History" << endl;
    for (auto &record : borrowHistory)
    {
        cout << "Resource : " << record.resource->getTitle() << endl;
        cout << "Borrowed : " << ctime(&record.borrowDate);

        if (record.returnDate != 0)
        {
            cout << "Returned : " << ctime(&record.returnDate);
            cout << "Fine     : " << record.fine << endl;
        }
        else
            cout << "Status   : Not Returned Yet" << endl;

        cout << "-----------------------" << endl;
    }
}

// ---------- Getters ----------
string Library::getLibraryName() const { return libraryName; }
vector<User *> &Library::getUsers() { return users; }
vector<LibraryResource *> &Library::getResources() { return resources; }
vector<Library::BorrowRecord> &Library::getBorrowHistory() { return borrowHistory; }