#include "../include/Admin.h"
#include "../include/Resources.h"
#include "../include/Library.h"
#include "../include/Utils.h"
#include "../include/FileHandler.h"
#include "../include/Colors.h"
#include <iostream>
#include <iomanip>
#include <fstream>
using namespace std;
using namespace Utils;

// ---------- Constructors ----------
Admin::Admin() : User()
{
    type = "admin";
}

Admin::Admin(int ID, string username, string password,
             string name, string address, double balance)
    : User(ID, "admin", username, password, name, address, balance)
{
}

// ---------- Role Rules ----------
int Admin::getDailyLimit() const { return 0; }    // admin does not borrow
double Admin::getFineRate() const { return 0.0; } // no fines for admin
int Admin::getBorrowDays() const { return 0; }    // admin does not borrow

// ---------- User Management ----------
void Admin::printAllCustomersReport(Library &lib)
{
    int choice;
    cout << "\n" << COLOR_SECTION << "What would you like to print?" << RESET << endl;
    cout << COLOR_MENU_OPTION << "1. Customers Info\n2. Borrowing History\n3. Both" << RESET << endl;
    choice = getValidInt(COLOR_INPUT_PROMPT "Enter your choice: " RESET);

    if (choice == 1 || choice == 3)
    {
        cout << "\n" << COLOR_SECTION << "--- Customers Report ---" << RESET << endl;

        cout << COLOR_TABLE_HEADER
             << left << setw(6)  << "ID"
             << setw(25) << "Name"
             << setw(12) << "Type"
             << setw(20) << "Username"
             << setw(10) << "Balance"
             << RESET << endl;

        cout << COLOR_SEPARATOR << string(73, '-') << RESET << endl;

        for (auto u : lib.getUsers())
        {
            if (!u->getIsDeleted())
            {
                cout << COLOR_ID      << left << setw(6)  << u->getUserID()  << RESET;
                cout << COLOR_TITLE   << setw(25) << u->getName()            << RESET;
                cout << COLOR_TYPE    << setw(12) << u->getType()            << RESET;
                cout << WHITE         << setw(20) << u->getUsername()        << RESET;
                cout << COLOR_BALANCE << setw(10) << u->getBalance()         << RESET;
                cout << endl;
            }
        }
    }

    if (choice == 2 || choice == 3)
    {
        lib.showBorrowHistory();
    }
}

void Admin::searchUser(Library &lib)
{
    int id;

    id = getValidInt(COLOR_INPUT_PROMPT "Enter user ID to search: " RESET);

    cout << "\n" << COLOR_SECTION << "--- User Search Result ---" << RESET << endl;

    cout << COLOR_TABLE_HEADER
         << left << setw(6)  << "ID"
         << setw(25) << "Name"
         << setw(12) << "Type"
         << setw(20) << "Username"
         << setw(10) << "Balance"
         << setw(10) << "Status"
         << RESET << endl;

    cout << COLOR_SEPARATOR << string(83, '-') << RESET << endl;

    bool found = false;
    for (auto u : lib.getUsers())
    {
        if (u->getUserID() == id)
        {
            cout << COLOR_ID      << left << setw(6)  << u->getUserID()  << RESET;
            cout << COLOR_TITLE   << setw(25) << u->getName()            << RESET;
            cout << COLOR_TYPE    << setw(12) << u->getType()            << RESET;
            cout << WHITE         << setw(20) << u->getUsername()        << RESET;
            cout << COLOR_BALANCE << setw(10) << u->getBalance()         << RESET;

            // Status — green = active, red = deleted
            if (u->getIsDeleted())
                cout << COLOR_DELETED << setw(10) << "Deleted" << RESET;
            else
                cout << COLOR_ACTIVE  << setw(10) << "Active"  << RESET;

            cout << endl;
            found = true;
            break;
        }
    }

    if (!found)
        cout << COLOR_ERROR << "User with ID " << id << " not found." << RESET << endl;
}

void Admin::fineManagement(Library &lib) {
    cout << "\n" << COLOR_SECTION << "--- Fine Management (Overdue Report) ---" << RESET << endl;

    cout << COLOR_TABLE_HEADER
         << left << setw(8)  << "User ID"
         << setw(25) << "Name"
         << setw(35) << "Resource"
         << setw(15) << "Days Overdue"
         << setw(10) << "Fine (Pending)"
         << RESET << endl;

    cout << COLOR_SEPARATOR << string(95, '-') << RESET << endl;

    time_t now = time(0);
    bool found = false;

    for (auto &record : lib.getBorrowHistory()) {
        // If resource not yet returned and overdue
        if (record.returnDate == 0 && difftime(now, record.dueDate) > 0) {
            int daysLate = static_cast<int>(difftime(now, record.dueDate) / (60 * 60 * 24));
            double fine = daysLate * lib.getUsers()[record.userID - 1]->getFineRate();

            // Store fine in record but do NOT deduct balance yet
            record.fine = fine;

            cout << COLOR_ID    << left << setw(8)  << record.userID                               << RESET;
            cout << COLOR_TITLE << setw(25) << lib.getUsers()[record.userID - 1]->getName()        << RESET;
            cout << WHITE       << setw(35) << record.resource->getTitle()                         << RESET;
            cout << BOLD_YELLOW << setw(15) << daysLate                                            << RESET;
            cout << COLOR_FINE  << setw(10) << fine                                                << RESET;
            cout << endl;

            found = true;
        }
    }

    if (!found) {
        cout << COLOR_SUCCESS << "No overdue resources at the moment." << RESET << endl;
    }
}


void Admin::deleteUser(Library &lib)
{
    int id;

    id = getValidInt(COLOR_INPUT_PROMPT "Enter user ID to delete: " RESET);

    for (auto u : lib.getUsers())
    {
        if (u->getUserID() == id)
        {
            if (u->getIsDeleted())
            {
                cout << COLOR_WARNING << "User already deleted." << RESET << endl;
                return;
            }
            u->markDeleted();
            cout << COLOR_WARNING << "User \"" << u->getName() << "\" marked as deleted." << RESET << endl;
            return;
        }
    }

    cout << COLOR_ERROR << "User with ID " << id << " not found." << RESET << endl;
}

// ---------- Resource Management ----------
void Admin::addResource(Library &lib)
{
    int choice;
    LibraryResource *newRes = nullptr;

    cout << COLOR_SECTION << "Select resource type to add:" << RESET << endl;
    cout << COLOR_MENU_OPTION
         << "1. Book\n2. DVD\n3. AudioBook\n4. Magazine\n5. Newspaper"
         << RESET << endl;
    choice = getValidInt(COLOR_INPUT_PROMPT "Enter choice: " RESET);
    int ID = generateNewResourceID("../database/resources.csv");
    int availableCopies;
    string title, author, category;

    title = getValidString(COLOR_INPUT_PROMPT "Enter title: " RESET);
    author = getValidString(COLOR_INPUT_PROMPT "Enter author/creator: " RESET);
    category = getValidString(COLOR_INPUT_PROMPT "Enter category: " RESET);
    availableCopies = getValidInt(COLOR_INPUT_PROMPT "Enter available copies: " RESET);

    if (choice == 1)
    {
        string ISBN, publisher;
        int year;
        ISBN = getValidString(COLOR_INPUT_PROMPT "Enter ISBN: " RESET);
        publisher = getValidString(COLOR_INPUT_PROMPT "Enter publisher: " RESET);
        year = getValidInt(COLOR_INPUT_PROMPT "Enter year published: " RESET);
        newRes = new Book(ID, title, author, category, availableCopies, ISBN, publisher, year);
        newRes->setTotalCopies(availableCopies);
    }
    else if (choice == 2)
    {
        string director, genre;
        int duration;
        director = getValidString(COLOR_INPUT_PROMPT "Enter director: " RESET);
        duration = getValidInt(COLOR_INPUT_PROMPT "Enter duration (minutes): " RESET);
        genre = getValidString(COLOR_INPUT_PROMPT "Enter genre: " RESET);
        newRes = new DVD(ID, title, director, category, availableCopies, duration, genre);
        newRes->setTotalCopies(availableCopies);
    }
    else if (choice == 3)
    {
        string narrator, format;
        int duration;
        narrator = getValidString(COLOR_INPUT_PROMPT "Enter narrator: " RESET);
        duration = getValidInt(COLOR_INPUT_PROMPT "Enter duration (minutes): " RESET);
        format = getValidString(COLOR_INPUT_PROMPT "Enter format (e.g. MP3, CD): " RESET);
        newRes = new AudioBook(ID, title, author, category, availableCopies, narrator, duration, format);
        newRes->setTotalCopies(availableCopies);
    }
    else if (choice == 4)
    {
        string publisher, pubDate;
        int volume, issue;
        publisher = getValidString(COLOR_INPUT_PROMPT "Enter publisher: " RESET);
        volume = getValidInt(COLOR_INPUT_PROMPT "Enter volume number: " RESET);
        issue = getValidInt(COLOR_INPUT_PROMPT "Enter issue number: " RESET);
        pubDate = getValidString(COLOR_INPUT_PROMPT "Enter publication date (e.g. May 2025): " RESET);
        newRes = new Magazine(ID, title, publisher, category, availableCopies, volume, issue, pubDate);
        newRes->setTotalCopies(availableCopies);
    }
    else if (choice == 5)
    {
        string publisher, editionDate, region;
        publisher = getValidString(COLOR_INPUT_PROMPT "Enter publisher: " RESET);
        editionDate = getValidString(COLOR_INPUT_PROMPT "Enter edition date (DD-MM-YYYY): " RESET);
        region = getValidString(COLOR_INPUT_PROMPT "Enter region: " RESET);
        newRes = new Newspaper(ID, title, publisher, category, availableCopies, editionDate, region);
        newRes->setTotalCopies(availableCopies);
    }
    else
    {
        cout << COLOR_ERROR << "Invalid choice. Resource not added." << RESET << endl;
        throw runtime_error("Invalid resource type choice!");
    }

    lib.addResource(newRes); // push into library's resources vector
    cout << COLOR_SUCCESS << "Resource added successfully." << RESET << endl;
}

void Admin::deleteResource(Library &lib)
{
    int id;

    id = getValidInt(COLOR_INPUT_PROMPT "Enter resource ID to delete: " RESET);

    for (auto r : lib.getResources())
    {
        if (r->getResourceID() == id)
        {
            if (r->getIsDeleted())
            {
                cout << COLOR_WARNING << "Resource already deleted." << RESET << endl;
                return;
            }
            r->markDeleted();
            cout << COLOR_WARNING << "Resource \"" << r->getTitle() << "\" marked as deleted." << RESET << endl;
            return;
        }
    }
    cout << COLOR_ERROR << "Resource with ID " << id << " not found." << RESET << endl;
}

void Admin::updateResource(Library &lib)
{
    int id;

    id = getValidInt(COLOR_INPUT_PROMPT "Enter resource ID to update: " RESET);

    for (auto r : lib.getResources())
    {
        if (r->getResourceID() == id)
        {
            cout << COLOR_SECTION << "Updating: " << r->getTitle() << RESET << endl;

            string newTitle, newAuthor, newCategory;
            int newCopies;

            newTitle = getValidString(COLOR_INPUT_PROMPT "Enter new title (leave blank to keep current): " RESET);
            newAuthor = getValidString(COLOR_INPUT_PROMPT "Enter new author/creator (leave blank to keep current): " RESET);
            newCategory = getValidString(COLOR_INPUT_PROMPT "Enter new category (leave blank to keep current): " RESET);
            newCopies = getValidInt(COLOR_INPUT_PROMPT "Enter new total copies (0 to keep current): " RESET);

            r->updateStatus(); // refresh availability status
            cout << COLOR_SUCCESS << "Resource updated successfully." << RESET << endl;
            return;
        }
    }

    cout << COLOR_ERROR << "Resource with ID " << id << " not found." << RESET << endl;
}

// ---------- Circulation / Borrowing Management ----------
void Admin::printIssuedResources(Library &lib)
{
    cout << "\n" << COLOR_SECTION << "--- Issued Resources ---" << RESET << endl;

    cout << COLOR_TABLE_HEADER
         << left << setw(6)  << "ID"
         << setw(35) << "Title"
         << setw(15) << "Type"
         << setw(15) << "Issued Copies"
         << RESET << endl;

    cout << COLOR_SEPARATOR << string(71, '-') << RESET << endl;

    bool found = false;
    for (auto r : lib.getResources())
    {
        if (r->getAvailableCopies() < r->getTotalCopies())
        {
            cout << COLOR_ID    << left << setw(6)  << r->getResourceID()                            << RESET;
            cout << COLOR_TITLE << setw(35) << r->getTitle()                                          << RESET;
            cout << COLOR_TYPE  << setw(15) << r->getType()                                           << RESET;
            cout << BOLD_YELLOW << setw(15) << (r->getTotalCopies() - r->getAvailableCopies())        << RESET;
            cout << endl;
            found = true;
        }
    }

    if (!found)
        cout << COLOR_SUCCESS << "No resources are currently issued." << RESET << endl;
}

void Admin::printOverdueResources(Library &lib)
{
    cout << "\n" << COLOR_SECTION << "--- Overdue Resources ---" << RESET << endl;

    cout << COLOR_TABLE_HEADER
         << left << setw(10) << "User ID"
         << setw(35) << "Resource"
         << setw(15) << "Days Overdue"
         << RESET << endl;

    cout << COLOR_SEPARATOR << string(60, '-') << RESET << endl;

    time_t now = time(0);
    bool found = false;

    for (const auto &record : lib.getBorrowHistory())
    {
        if (record.returnDate == 0 && difftime(now, record.dueDate) > 0)
        {
            cout << COLOR_ID    << left << setw(10) << record.userID             << RESET;
            cout << COLOR_TITLE << setw(35) << record.resource->getTitle()       << RESET;
            cout << BOLD_RED    << setw(15) << static_cast<int>(difftime(now, record.dueDate) / (60 * 60 * 24)) << RESET;
            cout << endl;
            found = true;
        }
    }

    if (!found)
        cout << COLOR_SUCCESS << "No overdue resources." << RESET << endl;
}

// ---------- Reports & Analytics ----------
void Admin::generateStats(Library &lib)
{
    cout << "\n" << COLOR_SECTION << "--- Library Statistics ---" << RESET << endl;

    // count only active (non-deleted) users
    int activeUsers = 0;
    for (auto u : lib.getUsers())
        if (!u->getIsDeleted())
            activeUsers++;

    // count only active (non-deleted) resources
    int activeResources = 0;
    for (auto r : lib.getResources())
        if (!r->getIsDeleted())
            activeResources++;

    cout << COLOR_TABLE_HEADER << left << setw(25) << "Total Users"     << RESET
         << BOLD_WHITE         << activeUsers                            << RESET << endl;

    cout << COLOR_TABLE_HEADER << left << setw(25) << "Total Resources" << RESET
         << BOLD_WHITE         << activeResources                        << RESET << endl;

    cout << COLOR_TABLE_HEADER << left << setw(25) << "Total Borrows"   << RESET
         << BOLD_WHITE         << lib.getBorrowHistory().size()          << RESET << endl;

    // find most borrowed resource — skip deleted resources
    int maxCount = 0;
    LibraryResource *mostBorrowed = nullptr;
    for (auto r : lib.getResources())
    {
        if (r->getIsDeleted())
            continue;

        int count = 0;
        for (const auto &record : lib.getBorrowHistory())
        {
            if (record.resource->getResourceID() == r->getResourceID())
                count++;
        }
        if (count > maxCount)
        {
            maxCount = count;
            mostBorrowed = r;
        }
    }

    if (mostBorrowed)
        cout << COLOR_TABLE_HEADER << left << setw(25) << "Most Borrowed" << RESET
             << COLOR_TITLE << mostBorrowed->getTitle()
             << BOLD_YELLOW << " (" << maxCount << " time(s))" << RESET << endl;
    else
        cout << COLOR_TABLE_HEADER << left << setw(25) << "Most Borrowed" << RESET
             << COLOR_WARNING << "No borrows recorded yet." << RESET << endl;
}

void Admin::exportReports(Library &lib, const string &filename)
{
    ofstream out(filename);

    if (!out.is_open())
    {
        throw runtime_error("Failed to open file for writing: " + filename);
    }

    // --- Issued Resources ---
    out << "\n--- Issued Resources ---" << endl;
    out << left << setw(6) << "ID"
        << setw(35) << "Title"
        << setw(15) << "Type"
        << setw(15) << "Issued Copies" << endl;
    out << string(71, '-') << endl;

    bool foundIssued = false;
    for (auto r : lib.getResources())
    {
        if (!r->getIsDeleted() && r->getAvailableCopies() < r->getTotalCopies())
        {
            out << left << setw(6) << r->getResourceID()
                << setw(35) << r->getTitle()
                << setw(15) << r->getType()
                << setw(15) << (r->getTotalCopies() - r->getAvailableCopies()) << endl;
            foundIssued = true;
        }
    }
    if (!foundIssued)
        out << "No resources are currently issued." << endl;

    // --- Overdue Resources ---
    out << "\n--- Overdue Resources ---" << endl;
    out << left << setw(10) << "User ID"
        << setw(35) << "Resource"
        << setw(15) << "Days Overdue" << endl;
    out << string(60, '-') << endl;

    time_t now = time(0);
    bool foundOverdue = false;
    for (const auto &record : lib.getBorrowHistory())
    {
        if (!record.resource->getIsDeleted() && record.returnDate == 0 && difftime(now, record.dueDate) > 0)
        {
            out << left << setw(10) << record.userID
                << setw(35) << record.resource->getTitle()
                << setw(15) << static_cast<int>(difftime(now, record.dueDate) / (60 * 60 * 24)) << endl;
            foundOverdue = true;
        }
    }
    if (!foundOverdue)
        out << "No overdue resources." << endl;

    out.close();
    cout << COLOR_SUCCESS << "Reports exported to " << filename << RESET << endl;
}

// ---------- Special / Additional Features ----------
void Admin::collectDonationFromUser(User *u, Library &lib)
{
    cout << "\n" << COLOR_SECTION << "--- Resource Donation ---" << RESET << endl;
    cout << COLOR_TITLE << "User: " << u->getName() << RESET << endl;

    // collect donated resource details
    addResource(lib); // reuse existing addResource logic

    // reward user balance based on donation
    double reward = 100.0; // fixed reward per donation
    u->updateBalance(reward);

    cout << COLOR_SUCCESS << "Balance rewarded: " << reward   << RESET << endl;
    cout << COLOR_SUCCESS << "New balance: "      << u->getBalance() << RESET << endl;
}

// ---------- Display ----------
void Admin::displayInfo() const
{
    cout << COLOR_TABLE_HEADER << "ID   : " << RESET << BOLD_WHITE << userID << RESET << endl;
    cout << COLOR_TABLE_HEADER << "Name : " << RESET << BOLD_WHITE << name   << RESET << endl;
    cout << COLOR_TABLE_HEADER << "Role : " << RESET << COLOR_TYPE << "Admin" << RESET << endl;
}