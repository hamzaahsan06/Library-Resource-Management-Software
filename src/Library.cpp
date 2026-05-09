#include "../include/Library.h"
#include "../include/FileHandler.h"
#include "../include/Utils.h"
#include "../include/Colors.h"
#include <iostream>
#include <iomanip>
#include <conio.h>
using namespace std;
using namespace Utils;

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

// adds new resource to library
void Library::addResource(LibraryResource *res)
{
    resources.push_back(res);
    cout << COLOR_SUCCESS << "Resource \"" << res->getTitle() << "\" added to " << libraryName << RESET << endl;
}

void Library::showResources() const
{
    cout << "\n"
         << COLOR_SECTION << "Resources in " << libraryName << RESET << endl;

    // Column headers — bold cyan like a textbook heading
    cout << COLOR_TABLE_HEADER
         << left << setw(6) << "ID"
         << setw(35) << "Title"
         << setw(15) << "Type"
         << setw(15) << "Available"
         << RESET << endl;

    // Separator — cyan divider line
    cout << COLOR_SEPARATOR << string(71, '-') << RESET << endl;

    for (auto res : resources)
    {
        if (!res->getIsDeleted()) // skip deleted resources
        {
            // ID in bold yellow — like a sticky note label
            cout << COLOR_ID << left << setw(6) << res->getResourceID() << RESET;
            // Title in bold white — most important, highest contrast
            cout << COLOR_TITLE << setw(35) << res->getTitle() << RESET;
            // Type in magenta — distinct category label
            cout << COLOR_TYPE << setw(15) << res->getType() << RESET;

            // Available copies — green if > 0, red if 0 (traffic light logic)
            int copies = res->getAvailableCopies();
            if (copies > 0)
                cout << COLOR_AVAILABLE << setw(15) << copies << RESET;
            else
                cout << COLOR_UNAVAILABLE << setw(15) << copies << RESET;

            cout << endl;
        }
    }

    cout << COLOR_SEPARATOR << string(71, '-') << RESET << endl;
}

// ---------- show only available resources ----------
void Library::showAvailableResources() const
{
    cout << "\n"
         << COLOR_MAIN_HEADER
         << "========== AVAILABLE RESOURCES =========="
         << RESET << endl;

    cout << COLOR_TABLE_HEADER
         << left << setw(6) << "ID"
         << setw(35) << "Title"
         << setw(15) << "Type"
         << setw(15) << "Available"
         << RESET << endl;

    cout << COLOR_SEPARATOR << string(71, '-') << RESET << endl;

    bool found = false;
    for (auto res : resources)
    {
        if (!res->getIsDeleted() && res->isAvailable())
        {
            cout << COLOR_ID << left << setw(6) << res->getResourceID() << RESET;
            cout << COLOR_TITLE << setw(35) << res->getTitle() << RESET;
            cout << COLOR_TYPE << setw(15) << res->getType() << RESET;
            cout << COLOR_AVAILABLE << setw(15) << res->getAvailableCopies() << RESET;
            cout << endl;
            found = true;
        }
    }

    if (!found)
        cout << COLOR_WARNING << "No resources currently available." << RESET << endl;

    cout << COLOR_SEPARATOR << string(71, '-') << RESET << endl;
}

// adds new user to library 
void Library::addUser(User *user)
{
    users.push_back(user);
    cout << COLOR_SUCCESS << "User \"" << user->getName() << "\" added to " << libraryName << RESET << endl;
}
// collects user info from input and adds to users vector
void Library::registerUser()
{
    int choice;
    User *u = nullptr;

    cout << COLOR_SECTION << "Select user type:" << RESET << endl;
    cout << COLOR_MENU_OPTION << "1. Student" << RESET << endl;
    cout << COLOR_MENU_OPTION << "2. Teacher" << RESET << endl;
    cout << COLOR_MENU_OPTION << "3. Staff" << RESET << endl;
    cout << COLOR_MENU_OPTION << "4. Premium Member" << RESET << endl;
    choice = getValidInt(COLOR_INPUT_PROMPT "Enter choice: " RESET);

    int id = generateNewUserID("../database/users.csv");

    string username, password, name, address;
    double balance;

    username = getValidString(COLOR_INPUT_PROMPT "Enter username: " RESET);
    password = getValidString(COLOR_INPUT_PROMPT "Enter password: " RESET);
    name = getValidString(COLOR_INPUT_PROMPT "Enter full name: " RESET);
    address = getValidString(COLOR_INPUT_PROMPT "Enter address: " RESET);
    balance = getValidDouble(COLOR_INPUT_PROMPT "Enter initial balance: " RESET);

    if (choice == 1)
    {
        string department;
        int rollNo;
        department = getValidString(COLOR_INPUT_PROMPT "Enter department: " RESET);
        rollNo = getValidInt(COLOR_INPUT_PROMPT "Enter roll number: " RESET);
        u = new Student(id, username, password, name, address, balance, department, rollNo);
    }
    else if (choice == 2)
    {
        string department, designation;
        department = getValidString(COLOR_INPUT_PROMPT "Enter department: " RESET);
        designation = getValidString(COLOR_INPUT_PROMPT "Enter designation: " RESET);
        u = new Teacher(id, username, password, name, address, balance, department, designation);
    }
    else if (choice == 3)
    {
        string position;
        position = getValidString(COLOR_INPUT_PROMPT "Enter position: " RESET);
        u = new Staff(id, username, password, name, address, balance, position);
    }
    else if (choice == 4)
    {
        int lvlChoice;
        string level;
        while (true)
        {
            cout << COLOR_SECTION << "Select membership level:" << RESET << "\n";
            cout << COLOR_MENU_OPTION << "1. Gold\n2. Silver\n3. Bronze" << RESET << "\n";
            lvlChoice = getValidInt(COLOR_INPUT_PROMPT "Enter choice: " RESET);
            if (lvlChoice == 1)
            {
                level = "Gold";
                break;
            }
            else if (lvlChoice == 2)
            {
                level = "Silver";
                break;
            }
            else if (lvlChoice == 3)
            {
                level = "Bronze";
                break;
            }
            else
                cerr << COLOR_ERROR << "Invalid choice. Please select 1, 2, or 3.\n"
                     << RESET;
        }
        cout << COLOR_SUCCESS << "You selected membership level: " << level << RESET << endl;
        u = new PremiumMember(id, username, password, name, address, balance, level);
    }
    else
    {
        throw runtime_error("Invalid choice. Please select 1-4!");
    }

    users.push_back(u);
    cout << COLOR_SUCCESS << "User registered successfully. ID: " << id << RESET << endl;
}

// searches users vector by username and password, returns matched user
void Library::searchResources() const
{
    // Display search options to the user
    cout << "\n"
         << COLOR_SECTION << "===== Search Resources =====" << RESET << "\n";
    cout << COLOR_MENU_OPTION
         << "1. ID\n2. Title\n3. Author\n4. Category\n5. Type"
         << RESET << "\n";

    int choice;
    choice = getValidInt(COLOR_INPUT_PROMPT "Enter choice: " RESET);

    // Take search keyword input
    string keyword;
    if (choice == 1)
        keyword = getValidString(COLOR_INPUT_PROMPT "Enter ID: " RESET);
    else if (choice == 2)
        keyword = getValidString(COLOR_INPUT_PROMPT "Enter Title: " RESET);
    else if (choice == 3)
        keyword = getValidString(COLOR_INPUT_PROMPT "Enter Author: " RESET);
    else if (choice == 4)
        keyword = getValidString(COLOR_INPUT_PROMPT "Enter Category: " RESET);
    else if (choice == 5)
        keyword = getValidString(COLOR_INPUT_PROMPT "Enter Type: " RESET);
    else
        throw runtime_error("Invalid choice. Please select 1-5!");

    // Convert keyword to lowercase for case-insensitive comparison
    for (auto &c : keyword)
        c = tolower(c);

    // Print table header in formatted style
    cout << COLOR_TABLE_HEADER
         << left << setw(6) << "ID"
         << setw(35) << "Title"
         << setw(15) << "Type"
         << setw(20) << "Author"
         << setw(20) << "Category"
         << setw(15) << "Available"
         << RESET << endl;

    cout << COLOR_SEPARATOR << string(111, '-') << RESET << endl;

    bool found = false; // Tracks if any match is found

    // Iterate through all resources in the library
    for (auto res : resources)
    {
        // Skip resources that are marked as deleted
        if (res->getIsDeleted())
            continue;

        string field;

        // Select the appropriate field based on user choice
        if (choice == 1)
            field = to_string(res->getResourceID());
        else if (choice == 2)
            field = res->getTitle();
        else if (choice == 3)
            field = res->getAuthorCreator();
        else if (choice == 4)
            field = res->getCategory();
        else if (choice == 5)
            field = res->getType();

        // Convert selected field to lowercase for fair comparison
        string fieldLower = field;
        for (auto &c : fieldLower)
            c = tolower(c);

        // Check if keyword exists inside the selected field
        if (fieldLower.find(keyword) != string::npos)
        {
            // Display matching resource in formatted output
            cout << COLOR_ID << left << setw(6) << res->getResourceID() << RESET;
            cout << COLOR_TITLE << setw(35) << res->getTitle() << RESET;
            cout << COLOR_TYPE << setw(15) << res->getType() << RESET;
            cout << WHITE << setw(20) << res->getAuthorCreator() << RESET;
            cout << WHITE << setw(20) << res->getCategory() << RESET;

            int copies = res->getAvailableCopies();
            if (copies > 0)
                cout << COLOR_AVAILABLE << setw(15) << copies << RESET;
            else
                cout << COLOR_UNAVAILABLE << setw(15) << copies << RESET;

            cout << endl;
            found = true;
        }
    }

    // If no matches found, inform the user
    if (!found)
        cout << COLOR_ERROR << "No resources found matching: " << keyword << RESET << endl;
}
// searches users vector by username and password, returns matched user
void Library::changePassword(User *u)
{
    string oldPass, newPass;

    oldPass = getValidString(COLOR_INPUT_PROMPT "Enter current password: " RESET);
    if (!u->login(u->getUsername(), oldPass))
    {
        throw runtime_error("Incorrect current password.");
    }
    newPass = oldPass;
    while (newPass == oldPass)
        newPass = getValidString(COLOR_INPUT_PROMPT "Enter new password: " RESET);

    u->setPassword(newPass);
    cout << COLOR_SUCCESS << "Password changed successfully." << RESET << endl;
}
// shows user's own details and borrow history
void Library::showUserProfile(User *u) const
{
    cout << "\n"
         << COLOR_SECTION << "========== " << u->getName() << " Profile ==========" << RESET << "\n";

    // Table header for user details
    cout << COLOR_TABLE_HEADER
         << left << setw(6) << "ID"
         << setw(25) << "Name"
         << setw(20) << "Username"
         << setw(12) << "Type"
         << setw(10) << "Balance"
         << RESET << endl;

    cout << COLOR_SEPARATOR << string(73, '-') << RESET << endl;

    // User data in tabular format
    cout << COLOR_ID << left << setw(6) << u->getUserID() << RESET;
    cout << COLOR_TITLE << setw(25) << u->getName() << RESET;
    cout << WHITE << setw(20) << u->getUsername() << RESET;
    cout << COLOR_TYPE << setw(12) << u->getType() << RESET;
    cout << COLOR_BALANCE << setw(10) << u->getBalance() << RESET;
    cout << endl;

    // Borrow history section
    cout << "\n"
         << COLOR_SECTION << "--- Borrow History ---" << RESET << endl;

    cout << COLOR_TABLE_HEADER
         << left << setw(8) << "User ID"
         << setw(35) << "Resource"
         << setw(14) << "Borrowed"
         << setw(14) << "Due"
         << setw(14) << "Returned"
         << setw(8) << "Fine"
         << RESET << endl;

    cout << COLOR_SEPARATOR << string(93, '-') << RESET << endl;

    bool found = false;

    auto formatDate = [](time_t t) -> string
    {
        if (t == 0)
            return "Not yet";
        struct tm *tm_info = localtime(&t);
        char buffer[11];
        strftime(buffer, sizeof(buffer), "%d-%m-%Y", tm_info);
        return string(buffer);
    };

    for (const auto &record : borrowHistory)
    {
        if (record.userID == u->getUserID()) // filter for specific user
        {
            cout << COLOR_ID << left << setw(8) << record.userID << RESET;
            cout << COLOR_TITLE << setw(35) << record.resource->getTitle() << RESET;
            cout << COLOR_DATE << setw(14) << formatDate(record.borrowDate) << RESET;
            cout << COLOR_DATE << setw(14) << formatDate(record.dueDate) << RESET;

            // Returned date — green if returned, yellow if still out
            if (record.returnDate != 0)
                cout << COLOR_AVAILABLE << setw(14) << formatDate(record.returnDate) << RESET;
            else
                cout << COLOR_WARNING << setw(14) << "Not yet" << RESET;

            // Fine — red if > 0, white if none
            if (record.fine > 0)
                cout << COLOR_FINE << setw(8) << record.fine << RESET;
            else
                cout << WHITE << setw(8) << record.fine << RESET;

            cout << endl;
            found = true;
        }
    }

    if (!found)
    {
        cout << COLOR_WARNING << "No borrow history found." << RESET << endl;
    }
}
// Process the deposit and immediately check for an upgrade
void Library::depositToUser(int userID, double amount)
{
    // Find the user in the library's master vector
    for (auto &uPtr : users)
    {
        if (uPtr->getUserID() == userID)
        {

            // Add the money
            uPtr->depositAmount(amount);

            // Check if this new balance triggers a Premium upgrade
            checkAndUpgradeUser(uPtr);
            return;
        }
    }
    throw runtime_error("User not found for deposit.");
}

// Grants the user membership status
void Library::checkAndUpgradeUser(User *&uPtr)
{
    // Only proceed if they are NOT already a Premium member and balance >= 500
    if (uPtr->getType() != "Premium" && uPtr->getBalance() >= 500.0)
    {

        // Determine sub-tier: 1000+ is Diamond, 500-999 is Gold
        string level = (uPtr->getBalance() >= 1000.0) ? "Diamond" : "Gold";

        // Hold onto the old object pointer to delete it later
        User *oldUser = uPtr;

        // Replace the pointer at this exact vector index with a new PremiumMember
        uPtr = new PremiumMember(
            oldUser->getUserID(),
            oldUser->getUsername(),
            oldUser->getPassword(), // Use getters or friend access if protected
            oldUser->getName(),
            oldUser->getAddress(),
            oldUser->getBalance(),
            level);

        // Delete the old Student/Teacher object to prevent memory leaks
        delete oldUser;

        // Notify the user of the system change
        cout << "\n" << COLOR_SEPARATOR << string(45, '=') << RESET << endl;
cout << BOLD_YELLOW << "  CONGRATULATIONS, " << uPtr->getName() << "!" << RESET << endl;
cout << COLOR_SUCCESS << "  You have been upgraded to "
     << BOLD_WHITE << level << " Premium Membership!" << RESET << endl;
cout << COLOR_SEPARATOR << string(45, '=') << RESET << "\n" << endl;
    }
}

User *Library::loginUser()
{
    string username, password = "";

    username = getValidString(COLOR_INPUT_PROMPT "Enter username: " RESET);

    cout << COLOR_INPUT_PROMPT << "Enter password: " << RESET;
    char ch;
    while (true)
    {
        ch = _getch();
        if (ch == '\r' || ch == '\n')
            break;
        else if (ch == '\b' && !password.empty())
        {
            password.pop_back();
            cout << "\b \b";
        }
        else if (ch != '\b')
        {
            password += ch;
            cout << '*';
        }
    }
    cout << endl;

    for (auto user : users)
    {
        if (!user->getIsDeleted() && user->login(username, password))
            return user;
    }

    cout << COLOR_ERROR << "Invalid username or password." << RESET << endl;
    return nullptr;
}

// borrows a resource for a user
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
        cout << COLOR_WARNING << user->getName() << " has reached the daily borrow limit of "
             << user->getDailyLimit() << " resources." << RESET << endl;
        return false;
    }

    // block borrow if no copies available
    if (!res->isAvailable())
    {
        cout << COLOR_UNAVAILABLE << "Resource \"" << res->getTitle() << "\" is not available." << RESET << endl;
        return false;
    }

    // decrease available copies by 1 in resource object
    res->borrowResource();

    // get borrow duration from user type and create borrow record
    borrowHistory.push_back(BorrowRecord(user->getUserID(), res, user->getBorrowDays()));

    cout << COLOR_SUCCESS << user->getName() << " successfully borrowed \"" << res->getTitle() << "\"." << RESET << endl;
    return true;
}

// returns a resource for a user, calculates fine if overdue, and updates borrow record
bool Library::returnResource(User *user, LibraryResource *res)
{
    bool everBorrowed = false;

    // search borrow history for a matching unreturned record
    for (auto &record : borrowHistory) // loop through each borrow record by reference so changes reflect in original
    {
        if (record.userID == user->getUserID() && // same user
            record.resource == res)               // same resource
        {
            everBorrowed = true;

            if (record.returnDate != 0) // not yet returned
            {
                cout << COLOR_WARNING << "\"" << res->getTitle() << "\" has already been returned." << RESET << endl;
                return false;
            }

            // set return date and calculate fine if overdue
            // fine is deducted from user balance inside markReturned
            record.markReturned(user);

            // increase available copies by 1 in resource object
            res->returnResource();

            cout << COLOR_SUCCESS << user->getName() << " returned \"" << res->getTitle() << "\"." << RESET << endl;

            if (record.fine > 0)
                cout << COLOR_FINE << "Overdue! Fine of " << record.fine << " deducted from balance." << RESET << endl;

            return true;
        }
    }

    // no active borrow record found for this user and resource
    if (!everBorrowed)
        cout << COLOR_ERROR << user->getName() << " has never borrowed \"" << res->getTitle() << "\"." << RESET << endl;

    return false;
}

// ---------- Borrow History ----------
void Library::showBorrowHistory() const
{
    cout << "\n"
         << COLOR_SECTION << "--- Borrow History ---" << RESET << endl;

    cout << COLOR_TABLE_HEADER
         << left << setw(8) << "User ID"
         << setw(35) << "Resource"
         << setw(14) << "Borrowed"
         << setw(14) << "Due"
         << setw(14) << "Returned"
         << setw(8) << "Fine"
         << RESET << endl;

    cout << COLOR_SEPARATOR << string(93, '-') << RESET << endl;

    if (borrowHistory.empty())
    {
        cout << COLOR_WARNING << "No borrow history found." << RESET << endl;
        return;
    }

    auto formatDate = [](time_t t) -> string
    {
        if (t == 0)
            return "Not yet";
        struct tm *tm_info = localtime(&t);
        char buffer[11];
        strftime(buffer, sizeof(buffer), "%d-%m-%Y", tm_info);
        return string(buffer);
    };

    for (const auto &record : borrowHistory)
    {
        cout << COLOR_ID << left << setw(8) << record.userID << RESET;
        cout << COLOR_TITLE << setw(35) << record.resource->getTitle() << RESET;
        cout << COLOR_DATE << setw(14) << formatDate(record.borrowDate) << RESET;
        cout << COLOR_DATE << setw(14) << formatDate(record.dueDate) << RESET;

        if (record.returnDate != 0)
            cout << COLOR_AVAILABLE << setw(14) << formatDate(record.returnDate) << RESET;
        else
            cout << COLOR_WARNING << setw(14) << "Not yet" << RESET;

        if (record.fine > 0)
            cout << COLOR_FINE << setw(8) << record.fine << RESET;
        else
            cout << WHITE << setw(8) << record.fine << RESET;

        cout << endl;
    }
}
// adds a new borrow record to the history vector
void Library::addBorrowRecord(int userID, LibraryResource *res, time_t borrowDate,
                              time_t dueDate, time_t returnDate, double fine, int durationDays)
{
    BorrowRecord record(userID, res, durationDays);
    record.borrowDate = borrowDate;
    record.dueDate = dueDate;
    record.returnDate = returnDate;
    record.fine = fine;
    record.durationDays = durationDays;
    borrowHistory.push_back(record);
}

// ---------- Getters ----------
string Library::getLibraryName() const { return libraryName; }
vector<User *> &Library::getUsers() { return users; }
vector<LibraryResource *> &Library::getResources() { return resources; }
vector<Library::BorrowRecord> &Library::getBorrowHistory() { return borrowHistory; }