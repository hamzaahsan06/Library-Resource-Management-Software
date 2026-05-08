#include <iostream>
#include <cstdlib>
#include "include/Library.h"
#include "include/Menu.h"
#include "include/FileHandler.h"
#include "include/Utils.h"
#include "include/Title.h"
#include "include/Colors.h"

using namespace std;
using namespace Utils;

int main()
{
    system("color");
    showTitle();
    Library lib("My Library");

    // Initialization: Load data from files
    try
    {
        ReadUsersFromFile("database/users.csv", lib.getUsers());
        ReadResourcesFromFile("database/resources.csv", lib.getResources());
        ReadBorrowHistoryFromFile("database/borrowHistory.csv", lib);
    }
    catch (const exception &e)
    {
        cout << COLOR_ERROR << "Error loading data: " << e.what() << RESET << endl;
        cout << COLOR_SYSTEM_NOTIFY << "Starting with empty database..." << RESET << endl;
    }

    // Launch the Library Management System UI
    runLibrarySystem(lib);

    // Finalization: Save all state to files
    try
    {
        SaveUsersToFile("database/users.csv", lib.getUsers());
        SaveResourcesToFile("database/resources.csv", lib.getResources());
        SaveBorrowHistoryToFile("database/borrowHistory.csv", lib);
        cout << COLOR_SUCCESS << "Data saved successfully." << RESET << endl;
    }
    catch (const exception &e)
    {
        cout << COLOR_ERROR << "Error saving data: " << e.what() << RESET << endl;
    }

    return 0;
}