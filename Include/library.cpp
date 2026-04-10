#include "Library.h"
#include <iostream>
#include <iomanip>
using namespace std;

// ===================== BorrowRecord =====================

Library::BorrowRecord::BorrowRecord(int uid, LibraryResource *res, int durationDays)
    : userID(uid), resource(res), fine(0.0), returnDate(0)
{
    borrowDate = time(0);
    dueDate = borrowDate + durationDays * 24 * 60 * 60;
}

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

Library::Library(string name) : libraryName(name) {}

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
    cout << "\nResources in " << libraryName << endl;
    cout << left << setw(6) << "ID"
         << setw(35) << "Title"
         << setw(15) << "Type"
         << setw(15) << "Available" << endl;
    cout << string(71, '-') << endl;

    for (auto res : resources)
    {
        if (!res->getIsDeleted())
        {
            cout << left << setw(6) << res->getResourceID()
                 << setw(35) << res->getTitle()
                 << setw(15) << res->getType()
                 << setw(15) << res->getAvailableCopies() << endl;
        }
    }
}

// ---------- Register User ----------
void Library::registerUser()
{
    int id;
    string name, username, password;

    cout << "\n===== Register User =====\n";

    cout << "Enter ID: ";
    cin >> id;
    cin.ignore();

    cout << "Enter Name: ";
    getline(cin, name);

    cout << "Enter Username: ";
    cin >> username;

    cout << "Enter Password: ";
    cin >> password;

    User* newUser = new User(id, name, username, password);
    users.push_back(newUser);

    cout << "User registered successfully!\n";
}

// ---------- Search Resources ----------
void Library::searchResources() const
{
    string keyword;

    cout << "\n===== Search Resources =====\n";
    cout << "Enter keyword: ";
    cin.ignore();
    getline(cin, keyword);

    bool found = false;

    for (auto res : resources)
    {
        if (!res->getIsDeleted() &&
            (res->getTitle().find(keyword) != string::npos ||
             res->getType().find(keyword) != string::npos))
        {
            cout << res->getResourceID() << " - "
                 << res->getTitle() << " ("
                 << res->getType() << ")" << endl;

            found = true;
        }
    }

    if (!found)
        cout << "No matching resources found.\n";
}

// ---------- Show Available Resources ----------
void Library::showAvailableResources() const
{
    cout << "\nAvailable Resources:\n";

    cout << left << setw(6) << "ID"
         << setw(35) << "Title"
         << setw(15) << "Type"
         << setw(15) << "Copies" << endl;

    cout << string(71, '-') << endl;

    bool found = false;

    for (auto res : resources)
    {
        if (!res->getIsDeleted() && res->getAvailableCopies() > 0)
        {
            cout << left << setw(6) << res->getResourceID()
                 << setw(35) << res->getTitle()
                 << setw(15) << res->getType()
                 << setw(15) << res->getAvailableCopies() << endl;

            found = true;
        }
    }

    if (!found)
        cout << "No available resources.\n";
}


// ---------- Borrowing Logic ----------
bool Library::borrowResource(User *user, LibraryResource *res)
{
    int countToday = 0;
    time_t now = time(0);

    for (auto &record : borrowHistory)
    {
        if (record.userID == user->getUserID() &&
            record.returnDate == 0 &&
            difftime(now, record.borrowDate) <= 24 * 60 * 60)
        {
            countToday++;
        }
    }

    if (countToday >= user->getDailyLimit())
    {
        cout << user->getName() << " has reached the daily borrow limit of "
             << user->getDailyLimit() << " resources." << endl;
        return false;
    }

    if (!res->isAvailable())
    {
        cout << "Resource \"" << res->getTitle() << "\" is not available." << endl;
        return false;
    }

    res->borrowResource();

    borrowHistory.push_back(BorrowRecord(user->getUserID(), res, user->getBorrowDays()));

    cout << user->getName() << " successfully borrowed \"" << res->getTitle() << "\"." << endl;
    return true;
}

// ---------- Returning Logic ----------
bool Library::returnResource(User *user, LibraryResource *res)
{
    for (auto &record : borrowHistory)
    {
        if (record.userID == user->getUserID() &&
            record.resource == res &&
            record.returnDate == 0)
        {
            record.markReturned(user);
            res->returnResource();

            cout << user->getName() << " returned \"" << res->getTitle() << "\"." << endl;

            if (record.fine > 0)
                cout << "Overdue! Fine of " << record.fine << " applied." << endl;

            return true;
        }
    }

    cout << user->getName() << " has not borrowed \"" << res->getTitle() << "\"." << endl;
    return false;
}

// ---------- Borrow History ----------
void Library::showBorrowHistory() const
{
    cout << "\nBorrow History" << endl;

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
        {
            cout << "Status   : Not Returned Yet" << endl;
        }

        cout << "-----------------------" << endl;
    }
}

// ---------- Getters ----------
string Library::getLibraryName() const { return libraryName; }
vector<User *> &Library::getUsers() { return users; }
vector<LibraryResource *> &Library::getResources() { return resources; }
vector<Library::BorrowRecord> &Library::getBorrowHistory() { return borrowHistory; }
