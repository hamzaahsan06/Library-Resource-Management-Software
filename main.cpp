#include <iostream>
#include <limits>
#include "sqlite3.h"
#include "header files/Users.h"
#include "header files/Resources.h"
#include "header files/Library.h"
#include "header files/admin.h"

using namespace std;

// Callback for SQLite SELECT
static int callback(void* NotUsed, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        cout << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << " | ";
    }
    cout << endl;
    return 0;
}

int main() {
    sqlite3* DB;
    int exit = sqlite3_open("library.db", &DB);

    if (exit) {
        cout << "Error opening database: " << sqlite3_errmsg(DB) << endl;
        return -1;
    }
    cout << "Database opened successfully!\n" << endl;

    char* errMsg;

    // --------- Create Library Resources ---------
    Book book1("C++ Basics", "Bjarne Stroustrup", "Programming", 5, "123-4567890123", "O'Reilly", 2020);
    DVD dvd1("Inception", "Christopher Nolan", "Movie", 3, 148, "Sci-Fi");
    EBook ebook1("Digital C++", "Author X", "Programming", "PDF", "link.com", "000-1111111111");

    // --------- Create Users ---------
    Student student1("student1", "pass123", 0, "Ali", "Khan", "Street 1", 101, "CS", 2026);
    Teacher teacher1("teacher1", "teach123", 0, "Sara", "Ali", "Street 2", 201, "CS", "Professor");
    Staff staff1("staff1", "staffpass", 0, "Zara", "Khan", "Street 3", 301, "Library", "Librarian");
    Admin admin1("admin1", "adminpass", 0, "Admin", "User", "HQ", 1);

    // --------- Borrow Resources ---------
    student1.borrowResource(&book1);
    teacher1.borrowResource(&dvd1);
    teacher1.borrowResource(&ebook1);
    staff1.borrowResource(&book1); // Staff borrows book
    staff1.borrowResource(&dvd1);  // Staff borrows DVD

    // --------- Display Info ---------
    cout << "\n--- User Info ---\n";
    student1.displayInfo();
    teacher1.displayInfo();
    staff1.displayInfo();
    admin1.displayInfo();

    // --------- Return Resource ---------
    student1.returnResource(&book1);
    teacher1.returnResource(&dvd1);

    cout << "\n--- Updated Info After Returns ---\n";
    student1.displayInfo();
    teacher1.displayInfo();

    // --------- Insert test user in SQLite ---------
    const char* sqlInsert =
        "INSERT INTO users (name, email, password, user_type, additional_details) "
        "VALUES ('Sara', 'sara@email.com', 'abc123', 'staff', '{\"department\":\"IT\",\"access\":\"partial\"}');";

    exit = sqlite3_exec(DB, sqlInsert, nullptr, 0, &errMsg);
    if (exit != SQLITE_OK) {
        cout << "Error inserting user: " << errMsg << endl;
        sqlite3_free(errMsg);
    } else {
        cout << "\nTest user inserted successfully!" << endl;
    }

    // --------- Read all users from SQLite ---------
    const char* sqlSelect = "SELECT * FROM users;";
    exit = sqlite3_exec(DB, sqlSelect, callback, 0, &errMsg);
    if (exit != SQLITE_OK) {
        cout << "Error reading users: " << errMsg << endl;
        sqlite3_free(errMsg);
    }

    sqlite3_close(DB);

    cout << "\nPress Enter to exit...";
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    cin.get();
    return 0;
}