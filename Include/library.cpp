#include "Library.h"
#include "../FileHandling/FileHandler.h"
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
        if (!res->getIsDeleted()) // skip deleted resources
        {
        cout << left << setw(6) << res->getResourceID()
             << setw(35) << res->getTitle()
             << setw(15) << res->getType()
             << setw(15) << res->getAvailableCopies() << endl;
        }
    }
}

// ---------- show only available resources ----------
void Library::showAvailableResources() const
{
    cout << "\n--- Available Resources ---" << endl;
    cout << left << setw(6) << "ID"
         << setw(35) << "Title"
         << setw(15) << "Type"
         << setw(15) << "Available" << endl;
    cout << string(71, '-') << endl;

    bool found = false;
    for (auto res : resources)
    {
        if (!res->getIsDeleted() && res->isAvailable())
        {
            cout << left << setw(6) << res->getResourceID()
                 << setw(35) << res->getTitle()
                 << setw(15) << res->getType()
                 << setw(15) << res->getAvailableCopies() << endl;
            found = true;
        }
    }

    if (!found)
        cout << "No resources currently available." << endl;
}

// ---------- User Management ----------
void Library::addUser(User *user)
{
    users.push_back(user);
    cout << "User \"" << user->getName() << "\" added to " << libraryName << endl;
}

void Library::registerUser()
{
    int choice;
    User *u = nullptr;
    
    try {
        cout << "Select user type:" << endl;
        cout << "1. Student" << endl;
        cout << "2. Teacher" << endl;
        cout << "3. Staff" << endl;
        cout << "4. Premium Member" << endl;
        cin >> choice;

        if(cin.fail())
            throw runtime_error("Invalid input. Expected an integer!");

        int id = generateNewUserID("../database/users.csv");

        string username, password, name, address;
        double balance;

        cout << "Enter username: ";
        cin >> username;
        if(username.empty())
            throw runtime_error("Username cannot be empty!");

        cout << "Enter password: ";
        cin >> password;
        if(password.empty())
            throw runtime_error("Password cannot be empty!");

        cout << "Enter full name: ";
        cin.ignore();
        getline(cin, name);
        if(name.empty())
            throw runtime_error("Name cannot be empty!");

        cout << "Enter address: ";
        getline(cin, address);
        if(address.empty())
            throw runtime_error("Address cannot be empty!");

        cout << "Enter initial balance: ";
        cin >> balance;
        if(cin.fail())
            throw runtime_error("Invalid input. Expected a number for balance!");

        if (choice == 1)
        {
            string department;
            int rollNo;
            
            cout << "Enter department: ";
            cin.ignore();
            getline(cin, department);
            if(department.empty())
                throw runtime_error("Department cannot be empty!");
            
            cout << "Enter roll number: ";
            cin >> rollNo;
            if(cin.fail())
                throw runtime_error("Invalid input. Expected an integer for roll number!");
            
            u = new Student(id, username, password, name, address, balance, department, rollNo);
        }
        else if (choice == 2)
        {
            string department, designation;
            
            cout << "Enter department: ";
            cin.ignore();
            getline(cin, department);
            if(department.empty())
                throw runtime_error("Department cannot be empty!");
            
            cout << "Enter designation: ";
            getline(cin, designation);
            if(designation.empty())
                throw runtime_error("Designation cannot be empty!");
            
            u = new Teacher(id, username, password, name, address, balance, department, designation);
        }
        else if (choice == 3)
        {
            string position;
            
            cout << "Enter position: ";
            cin.ignore();
            getline(cin, position);
            if(position.empty())
                throw runtime_error("Position cannot be empty!");
            
            u = new Staff(id, username, password, name, address, balance, position);
        }
        else if (choice == 4)
        {
            string level;
            
            cout << "Enter membership level (Gold/Silver/Bronze): ";
            cin >> level;
            if(level.empty())
                throw runtime_error("Membership level cannot be empty!");
            
            if(level != "Gold" && level != "Silver" && level != "Bronze")
                throw runtime_error("Invalid input. Only enter (Gold/Silver/Bronze)");
            
            u = new PremiumMember(id, username, password, name, address, balance, level);
        }
        else
        {
            throw runtime_error("Invalid choice. Please select 1-4!");
        }

        users.push_back(u);
        cout << "User registered successfully. ID: " << id << endl;
    }
    catch(const runtime_error& e) {
        cout << "Error: " << e.what() << endl;
        delete u;
        cin.clear();
        cin.ignore(1000, '\n');
    }
}

// ---------- Search ----------
void Library::searchResources() const
{
    try {
        // Display search options to the user
        cout << "\n===== Search Resources =====\n";
        cout << "Search by:\n";
        cout << "1. Title\n";
        cout << "2. Author\n";
        cout << "3. Category\n";
        cout << "4. Type\n";

        int choice;
        cin >> choice;
        if(cin.fail())
            throw runtime_error("Invalid input. Expected an integer!");

        // Take search keyword input
        string keyword;
        cout << "Enter search keyword: ";
        cin.ignore();              // Clear leftover newline from input buffer
        getline(cin, keyword);     // Allows multi-word input
        if(keyword.empty())
            throw runtime_error("Search keyword cannot be empty!");

        // Convert keyword to lowercase for case-insensitive comparison
        for (auto &c : keyword)
            c = tolower(c);

        // Print table header in formatted style
        cout << left << setw(6)  << "ID"
             << setw(35) << "Title"
             << setw(15) << "Type"
             << setw(20) << "Author"
             << setw(15) << "Available" << endl;

        cout << string(91, '-') << endl;

        bool found = false; // Tracks if any match is found

        // Iterate through all resources in the library
        for (auto res : resources)
        {
            // Skip resources that are marked as deleted
            if (res->getIsDeleted())
                continue;

            string field;

            // Select the appropriate field based on user choice
            if      (choice == 1) field = res->getTitle();
            else if (choice == 2) field = res->getAuthorCreator();
            else if (choice == 3) field = res->getCategory();
            else if (choice == 4) field = res->getType();
            else
                throw runtime_error("Invalid choice. Please select 1-4!");

            // Convert selected field to lowercase for fair comparison
            for (auto &c : field)
                c = tolower(c);

            // Check if keyword exists inside the selected field
            if (field.find(keyword) != string::npos)
            {
                // Display matching resource in formatted output
                cout << left << setw(6)  << res->getResourceID()
                     << setw(35) << res->getTitle()
                     << setw(15) << res->getType()
                     << setw(20) << res->getAuthorCreator()
                     << setw(15) << res->getAvailableCopies() << endl;

                found = true;
            }
        }

        // If no matches found, inform the user
        if (!found)
            cout << "No resources found matching: " << keyword << endl;
    }
    catch(const runtime_error& e) {
        cout << "Error: " << e.what() << endl;
        cin.clear();
        cin.ignore(1000, '\n');
    }
}

void Library::changePassword(User *u)
{
    string oldPass, newPass;
    
    try {
        cout << "Enter current password: ";
        cin >> oldPass;
        if(oldPass.empty())
            throw runtime_error("Password cannot be empty!");

        if (!u->login(u->getUsername(), oldPass))
        {
            cout << "Incorrect current password." << endl;
            return;
        }

        cout << "Enter new password: ";
        cin >> newPass;
        if(newPass.empty())
            throw runtime_error("New password cannot be empty!");
        
        u->setPassword(newPass);
        cout << "Password changed successfully." << endl;
    }
    catch(const runtime_error& e) {
        cout << "Error: " << e.what() << endl;
        cin.clear();
        cin.ignore(1000, '\n');
    }
}

void Library::showUserProfile(User *u) const
{
    cout << "\n========== " << u->getName() << " Profile ==========\n";

    // Table header for user details
    cout << left << setw(6)  << "ID"
         << setw(25) << "Name"
         << setw(20) << "Username"
         << setw(12) << "Type"
         << setw(10) << "Balance" << endl;

    cout << string(73, '-') << endl;

    // User data in tabular format
    cout << left << setw(6)  << u->getUserID()
         << setw(25) << u->getName()
         << setw(20) << u->getUsername()
         << setw(12) << u->getType()
         << setw(10) << u->getBalance() << endl;

    // Borrow history section
    cout << "\n========== My Borrow History ==========\n";

    cout << left << setw(8)  << "UserID"
         << setw(30) << "Resource"
         << setw(25) << "Borrowed"
         << setw(25) << "Due"
         << setw(15) << "Status"
         << setw(8)  << "Fine" << endl;

    cout << string(111, '-') << endl;

    bool found = false;

    for (const auto &record : borrowHistory)
    {
        if (record.userID == u->getUserID())
        {
            // Format dates properly 
            string borrowed = ctime(&record.borrowDate);
            string due = ctime(&record.dueDate);

            // Remove newline added by ctime
            borrowed.pop_back();
            due.pop_back();

            string status;

            if (record.returnDate == 0)
            {
                status = "Not Returned";
            }
            else
            {
                status = "Returned";
            }

            cout << left << setw(8)  << record.userID
                 << setw(30) << record.resource->getTitle()
                 << setw(25) << borrowed
                 << setw(25) << due
                 << setw(15) << status
                 << setw(8)  << record.fine << endl;

            found = true;
        }
    }

    if (!found)
    {
        cout << "No borrow history found.\n";
    }
}


User *Library::loginUser()
{
    string username, password;
    
    try {
        cout << "Enter username: ";
        cin >> username;
        if(username.empty())
            throw runtime_error("Username cannot be empty!");
        
        cout << "Enter password: ";
        cin >> password;
        if(password.empty())
            throw runtime_error("Password cannot be empty!");

        for (auto user : users)
        {
            if (!user->getIsDeleted() && user->login(username, password))
                return user; // returns pointer to actual object in vector
        }

        cout << "Invalid username or password." << endl;
        return nullptr;
    }
    catch(const runtime_error& e) {
        cout << "Error: " << e.what() << endl;
        cin.clear();
        cin.ignore(1000, '\n');
        return nullptr;
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

void Library::addBorrowRecord(int userID, LibraryResource *res, time_t borrowDate,
                              time_t dueDate, time_t returnDate, double fine, int durationDays)
{
    BorrowRecord record(userID, res, durationDays);
    record.borrowDate  = borrowDate;
    record.dueDate     = dueDate;
    record.returnDate  = returnDate;
    record.fine        = fine;
    borrowHistory.push_back(record);
}
// ---------- Getters ----------
string Library::getLibraryName() const { return libraryName; }
vector<User *> &Library::getUsers() { return users; }
vector<LibraryResource *> &Library::getResources() { return resources; }
vector<Library::BorrowRecord> &Library::getBorrowHistory() { return borrowHistory; }
