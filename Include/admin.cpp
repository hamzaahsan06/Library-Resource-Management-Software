#include "admin.h"
#include "Resources.h"
#include "Library.h"
#include "utils.h"
#include "../FileHandling/FileHandler.h"
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
    cout << "\nWhat would you like to print?" << endl;
    cout << "1. Customers Info" << endl;
    cout << "2. Borrowing History" << endl;
    cout << "3. Both" << endl;
    choice = getValidInt("Enter your choice: ");

    if (choice == 1 || choice == 3)
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
            if (!u->getIsDeleted())
            {
                cout << left << setw(6) << u->getUserID()
                     << setw(25) << u->getName()
                     << setw(12) << u->getType()
                     << setw(20) << u->getUsername()
                     << setw(10) << u->getBalance() << endl;
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

    id = getValidInt("Enter user ID to search: ");

    cout << "\n--- User Search Result ---" << endl;

    cout << left << setw(6) << "ID"
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
            cout << left << setw(6) << u->getUserID()
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

void Admin::deleteUser(Library &lib)
{
    int id;

    id = getValidInt("Enter user ID to delete: ");

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

// ---------- Resource Management ----------
void Admin::addResource(Library &lib)
{
    int choice;
    LibraryResource *newRes = nullptr;

    cout << "Select resource type to add:" << endl;
    cout << "1. Book" << endl;
    cout << "2. DVD" << endl;
    cout << "3. AudioBook" << endl;
    cout << "4. Magazine" << endl;
    cout << "5. Newspaper" << endl;
    choice = getValidInt("Enter choice: ");
    int ID = generateNewResourceID("../database/resources.csv");
    int availableCopies;
    string title, author, category;

    title = getValidString("Enter title: ");

    author = getValidString("Enter author/creator: ");

    category = getValidString("Enter category: ");
    if (category.empty())
        throw runtime_error("Category cannot be empty!");

    availableCopies = getValidInt("Enter available copies: ");

    if (choice == 1)
    {
        string ISBN, publisher;
        int year;
        ISBN = getValidString("Enter ISBN: ");
        publisher = getValidString("Enter publisher: ");
        year = getValidInt("Enter year published: ");
        newRes = new Book(ID, title, author, category, availableCopies, ISBN, publisher, year);
        newRes->setTotalCopies(availableCopies);
    }
    else if (choice == 2)
    {
        string director, genre;
        int duration;
        director = getValidString("Enter director: ");
        duration = getValidInt("Enter duration (minutes): ");
        genre = getValidString("Enter genre: ");
        newRes = new DVD(ID, title, director, category, availableCopies, duration, genre);
        newRes->setTotalCopies(availableCopies);
    }
    else if (choice == 3)
    {
        string narrator, format;
        int duration;
        narrator = getValidString("Enter narrator: ");
        duration = getValidInt("Enter duration (minutes): ");
        format = getValidString("Enter format (e.g. MP3, CD): ");
        newRes = new AudioBook(ID, title, author, category, availableCopies, narrator, duration, format);
        newRes->setTotalCopies(availableCopies);
    }
    else if (choice == 4)
    {
        string publisher, pubDate;
        int volume, issue;
        publisher = getValidString("Enter publisher: ");
        volume = getValidInt("Enter volume number: ");
        issue = getValidInt("Enter issue number: ");
        pubDate = getValidString("Enter publication date (e.g. May 2025): ");
        newRes = new Magazine(ID, title, publisher, category, availableCopies, volume, issue, pubDate);
        newRes->setTotalCopies(availableCopies);
    }
    else if (choice == 5)
    {
        string publisher, editionDate, region;
        publisher = getValidString("Enter publisher: ");
        editionDate = getValidString("Enter edition date (DD-MM-YYYY): ");
        region = getValidString("Enter region: ");
        newRes = new Newspaper(ID, title, publisher, category, availableCopies, editionDate, region);
        newRes->setTotalCopies(availableCopies);
    }
    else
    {
        cout << "Invalid choice. Resource not added." << endl;
        throw runtime_error("Invalid resource type choice!");
    }

    lib.addResource(newRes); // push into library's resources vector
    cout << "Resource added successfully." << endl;
}

void Admin::deleteResource(Library &lib)
{
    int id;

    id = getValidInt("Enter resource ID to delete: ");

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

    id = getValidInt("Enter resource ID to update: ");

    for (auto r : lib.getResources())
    {
        if (r->getResourceID() == id)
        {
            cout << "Updating: " << r->getTitle() << endl;

            string newTitle, newAuthor, newCategory;
            int newCopies;

            newTitle = getValidString("Enter new title (leave blank to keep current): ");
            if (!newTitle.empty())
                r->setTitle(newTitle);

            newAuthor = getValidString("Enter new author/creator (leave blank to keep current): ");
            if (!newAuthor.empty())
                r->setAuthorCreator(newAuthor);

            newCategory = getValidString("Enter new category (leave blank to keep current): ");
            if (!newCategory.empty())
                r->setCategory(newCategory);

            newCopies = getValidInt("Enter new total copies (0 to keep current): ");
            if (newCopies > 0)
                r->setTotalCopies(newCopies);

            r->updateStatus(); // refresh availability status
            cout << "Resource updated successfully." << endl;
            return;
        }
    }

    cout << "Resource with ID " << id << " not found." << endl;
}

// ---------- Circulation / Borrowing Management ----------
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

// ---------- Reports & Analytics ----------
void Admin::generateStats(Library &lib)
{
    cout << "\n--- Library Statistics ---" << endl;

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

    cout << left << setw(25) << "Total Users" << activeUsers << endl;
    cout << left << setw(25) << "Total Resources" << activeResources << endl;
    cout << left << setw(25) << "Total Borrows" << lib.getBorrowHistory().size() << endl;

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
        cout << left << setw(25) << "Most Borrowed"
             << mostBorrowed->getTitle() << " (" << maxCount << " time(s))" << endl;
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

// ---------- Special / Additional Features ----------
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

// ---------- Display ----------
void Admin::displayInfo() const
{
    cout << "ID   : " << userID << endl;
    cout << "Name : " << name << endl;
    cout << "Role : Admin" << endl;
}
