#include "admin.h"
#include "Resources.h"
#include "Library.h"
#include "../FileHandling/FileHandler.h"
#include <iostream>
#include <iomanip>
#include <fstream>
using namespace std;

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

// ---------- Admin Operations ----------

void Admin::printAllCustomersReport(Library &lib)
{
    cout << "\n--- Customers Report ---" << endl;
    cout << left << setw(6) << "ID"
         << setw(25) << "Name"
         << setw(12) << "Type"
         << setw(20) << "Username"
         << setw(10) << "Balance" << endl;
    cout << string(73, '-') << endl;

    for (auto u : lib.getUsers())
    {
        if (!u->getIsDeleted()) // skip deleted users
        {
            cout << left << setw(6) << u->getUserID()
                 << setw(25) << u->getName()
                 << setw(12) << u->getType()
                 << setw(20) << u->getUsername()
                 << setw(10) << u->getBalance() << endl;
        }
    }

    cout << "\n--- Borrowing History ---" << endl;
    cout << left << setw(8) << "User ID"
         << setw(30) << "Resource"
         << setw(25) << "Borrowed"
         << setw(25) << "Due"
         << setw(15) << "Returned"
         << setw(8) << "Fine" << endl;
    cout << string(111, '-') << endl;

    for (const auto &record : lib.getBorrowHistory())
    {
        // ctime adds newline at end so we strip it for clean output
        string borrowed = ctime(&record.borrowDate);
        string due = ctime(&record.dueDate);
        string returned = record.returnDate == 0 ? "Not yet" : ctime(&record.returnDate);

        borrowed.pop_back(); // remove trailing newline from ctime
        due.pop_back();
        if (record.returnDate != 0)
            returned.pop_back();

        cout << left << setw(8) << record.userID
             << setw(30) << record.resource->getTitle()
             << setw(25) << borrowed
             << setw(25) << due
             << setw(15) << returned
             << setw(8) << record.fine << endl;
    }
}

void Admin::deleteUser(Library &lib)
{
    int id;
    cout << "Enter user ID to delete: ";
    cin >> id;

    for (auto u : lib.getUsers())
    {
        if (u->getUserID() == id)
        {
            if (u->getIsDeleted())
            {
                cout << "User already deleted." << endl;
                return;
            }
            u->markDeleted();
            cout << "User \"" << u->getName() << "\" marked as deleted." << endl;
            return;
        }
    }
    cout << "User with ID " << id << " not found." << endl;
}

void Admin::addResource(Library &lib)
{
    int choice;
    cout << "Select resource type to add:" << endl;
    cout << "1. Book" << endl;
    cout << "2. DVD" << endl;
    cout << "3. AudioBook" << endl;
    cout << "4. Magazine" << endl;
    cout << "5. Newspaper" << endl;
    cin >> choice;

    // auto generate ID from last resource in vector
    int ID = generateNewResourceID("../database/resources.csv");
    int totalCopies;
    string title, author, category;

    cout << "Enter title: ";
    cin.ignore();
    getline(cin, title);
    cout << "Enter author/creator: ";
    getline(cin, author);
    cout << "Enter category: ";
    getline(cin, category);
    cout << "Enter total copies: ";
    cin >> totalCopies;

    LibraryResource *newRes = nullptr;

    if (choice == 1)
    {
        string ISBN, publisher;
        int year;
        cout << "Enter ISBN: ";
        cin >> ISBN;
        cout << "Enter publisher: ";
        cin.ignore();
        getline(cin, publisher);
        cout << "Enter year published: ";
        cin >> year;
        newRes = new Book(ID, title, author, category, totalCopies, ISBN, publisher, year);
    }
    else if (choice == 2)
    {
        string director, genre;
        int duration;
        cout << "Enter director: ";
        cin.ignore();
        getline(cin, director);
        cout << "Enter duration (minutes): ";
        cin >> duration;
        cout << "Enter genre: ";
        cin.ignore();
        getline(cin, genre);
        newRes = new DVD(ID, title, director, category, totalCopies, duration, genre);
    }
    else if (choice == 3)
    {
        string narrator, format;
        int duration;
        cout << "Enter narrator: ";
        cin.ignore();
        getline(cin, narrator);
        cout << "Enter duration (minutes): ";
        cin >> duration;
        cout << "Enter format (MP3/CD): ";
        cin.ignore();
        getline(cin, format);
        newRes = new AudioBook(ID, title, author, category, totalCopies, narrator, duration, format);
    }
    else if (choice == 4)
    {
        string publisher, pubDate;
        int volume, issue;
        cout << "Enter publisher: ";
        cin.ignore();
        getline(cin, publisher);
        cout << "Enter volume number: ";
        cin >> volume;
        cout << "Enter issue number: ";
        cin >> issue;
        cout << "Enter publication date (e.g. May 2025): ";
        cin.ignore();
        getline(cin, pubDate);
        newRes = new Magazine(ID, title, publisher, category, totalCopies, volume, issue, pubDate);
    }
    else if (choice == 5)
    {
        string publisher, editionDate, region;
        cout << "Enter publisher: ";
        cin.ignore();
        getline(cin, publisher);
        cout << "Enter edition date (DD-MM-YYYY): ";
        getline(cin, editionDate);
        cout << "Enter region: ";
        getline(cin, region);
        newRes = new Newspaper(ID, title, publisher, category, totalCopies, editionDate, region);
    }
    else
    {
        cout << "Invalid choice. Resource not added." << endl;
        return;
    }

    lib.addResource(newRes); // push into library's resources vector
    cout << "Resource added successfully." << endl;
}

void Admin::deleteResource(Library &lib)
{
    int id;
    cout << "Enter resource ID to delete: ";
    cin >> id;

    for (auto r : lib.getResources())
    {
        if (r->getResourceID() == id)
        {
            if (r->getIsDeleted())
            {
                cout << "Resource already deleted." << endl;
                return;
            }
            r->markDeleted();
            cout << "Resource \"" << r->getTitle() << "\" marked as deleted." << endl;
            return;
        }
    }
    cout << "Resource with ID " << id << " not found." << endl;
}

void Admin::updateResource(Library &lib)
{
    int id;
    cout << "Enter resource ID to update: ";
    cin >> id;

    for (auto r : lib.getResources())
    {
        if (r->getResourceID() == id)
        {
            cout << "Updating: " << r->getTitle() << endl;

            string newTitle, newAuthor, newCategory;
            int newCopies;

            cout << "Enter new title (leave blank to keep current): ";
            cin.ignore();
            getline(cin, newTitle);
            if (!newTitle.empty())
                r->setTitle(newTitle);

            cout << "Enter new author/creator (leave blank to keep current): ";
            getline(cin, newAuthor);
            if (!newAuthor.empty())
                r->setAuthorCreator(newAuthor);

            cout << "Enter new category (leave blank to keep current): ";
            getline(cin, newCategory);
            if (!newCategory.empty())
                r->setCategory(newCategory);

            cout << "Enter new total copies (0 to keep current): ";
            cin >> newCopies;
            if (newCopies > 0)
                r->setTotalCopies(newCopies);

            r->updateStatus(); // refresh availability status
            cout << "Resource updated successfully." << endl;
            return;
        }
    }

    cout << "Resource with ID " << id << " not found." << endl;
}

void Admin::printIssuedResources(Library &lib)
{
    cout << "\n--- Issued Resources ---" << endl;
    cout << left << setw(6) << "ID"
         << setw(35) << "Title"
         << setw(15) << "Type"
         << setw(15) << "Issued Copies" << endl;
    cout << string(71, '-') << endl;

    bool found = false;
    for (auto r : lib.getResources())
    {
        if (r->getAvailableCopies() < r->getTotalCopies())
        {
            cout << left << setw(6) << r->getResourceID()
                 << setw(35) << r->getTitle()
                 << setw(15) << r->getType()
                 << setw(15) << (r->getTotalCopies() - r->getAvailableCopies()) << endl;
            found = true;
        }
    }

    if (!found)
        cout << "No resources are currently issued." << endl;
}

void Admin::collectDonationFromUser(User *u, Library &lib)
{
    cout << "\n--- Resource Donation ---" << endl;
    cout << "User: " << u->getName() << endl;

    // collect donated resource details
    addResource(lib); // reuse existing addResource logic

    // reward user balance based on donation
    double reward = 100.0; // fixed reward per donation
    u->updateBalance(reward);

    cout << "Balance rewarded: " << reward << endl;
    cout << "New balance: " << u->getBalance() << endl;
}

void Admin::searchUser(Library &lib)
{
    int id;
    cout << "Enter user ID to search: ";
    cin >> id;

    cout << left << setw(6)  << "ID"
                 << setw(25) << "Name"
                 << setw(12) << "Type"
                 << setw(20) << "Username"
                 << setw(10) << "Balance"
                 << setw(10) << "Status" << endl;
    cout << string(83, '-') << endl;

    bool found = false;
    for (auto u : lib.getUsers())
    {
        if (u->getUserID() == id)
        {
            cout << left << setw(6)  << u->getUserID()
                         << setw(25) << u->getName()
                         << setw(12) << u->getType()
                         << setw(20) << u->getUsername()
                         << setw(10) << u->getBalance()
                         << setw(10) << (u->getIsDeleted() ? "Deleted" : "Active") << endl;
            found = true;
            break;
        }
    }

    if (!found)
        cout << "User with ID " << id << " not found." << endl;
}

void Admin::printOverdueResources(Library &lib)
{
    cout << "\n--- Overdue Resources ---" << endl;
    cout << left << setw(10) << "User ID"
         << setw(35) << "Resource"
         << setw(15) << "Days Overdue" << endl;
    cout << string(60, '-') << endl;

    time_t now = time(0);
    bool found = false;

    for (const auto &record : lib.getBorrowHistory())
    {
        if (record.returnDate == 0 && difftime(now, record.dueDate) > 0)
        {
            cout << left << setw(10) << record.userID
                 << setw(35) << record.resource->getTitle()
                 << setw(15) << static_cast<int>(difftime(now, record.dueDate) / (60 * 60 * 24)) << endl;
            found = true;
        }
    }

    if (!found)
        cout << "No overdue resources." << endl;
}

void Admin::generateStats(Library &lib)
{
    cout << "\n--- Library Statistics ---" << endl;
    cout << left << setw(25) << "Total Users" << lib.getUsers().size() << endl;
    cout << left << setw(25) << "Total Resources" << lib.getResources().size() << endl;
    cout << left << setw(25) << "Total Borrows" << lib.getBorrowHistory().size() << endl;

    // find most borrowed resource
    int maxCount = 0;
    LibraryResource *mostBorrowed = nullptr;

    for (auto r : lib.getResources())
    {
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
        cout << left << setw(25) << "Most Borrowed"
             << mostBorrowed->getTitle() << " (" << maxCount << " times)" << endl;
    else
        cout << left << setw(25) << "Most Borrowed" << "No borrows recorded yet." << endl;
}

void Admin::exportReports(Library &lib, const string &filename)
{
    ofstream out(filename);

    if (!out.is_open())
    {
        cout << "Error: Could not open " << filename << endl;
        return;
    }

    out << "--- Issued Resources ---" << endl;
    for (auto r : lib.getResources())
    {
        if (r->getAvailableCopies() < r->getTotalCopies())
        {
            out << r->getTitle() << " | Issued: "
                << (r->getTotalCopies() - r->getAvailableCopies()) << " copies" << endl;
        }
    }

    out << endl
        << "--- Overdue Resources ---" << endl;
    time_t now = time(0);
    for (const auto &record : lib.getBorrowHistory())
    {
        if (record.returnDate == 0 && difftime(now, record.dueDate) > 0)
        {
            out << "User ID: " << record.userID
                << " | Resource: " << record.resource->getTitle()
                << " | Days Overdue: "
                << static_cast<int>(difftime(now, record.dueDate) / (60 * 60 * 24)) << endl;
        }
    }

    out.close();
    cout << "Reports exported to " << filename << endl;
}

void Admin::fineManagement(Library &lib)
{
    cout << "\n--- Fine Management ---" << endl;
    cout << left << setw(6) << "ID"
         << setw(25) << "Name"
         << setw(10) << "Balance" << endl;
    cout << string(41, '-') << endl;

    for (auto u : lib.getUsers())
    {
        cout << left << setw(6) << u->getUserID()
             << setw(25) << u->getName()
             << setw(10) << u->getBalance() << endl;
    }
}

// ---------- Display ----------
void Admin::displayInfo() const
{
    cout << "ID   : " << userID << endl;
    cout << "Name : " << name << endl;
    cout << "Role : Admin" << endl;
}
