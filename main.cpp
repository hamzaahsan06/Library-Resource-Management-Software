#include <iostream>
#include <cstdlib>
#include "include/Library.h"
#include "include/admin.h"
#include "FileHandling/FileHandler.h"
#include "Include/utils.h"
#include "include/title.h"
#include "main_menu/admin_menu.h"
#include "main_menu/library_menu.h"
#include "main_menu/clearANDpause.h"

using namespace std;
using namespace Utils;

int main()
{
    showTitle();

    Library lib("My Library");

    try
    {
        ReadUsersFromFile("database/users.csv", lib.getUsers());
        ReadResourcesFromFile("database/resources.csv", lib.getResources());
        ReadBorrowHistoryFromFile("database/borrowHistory.csv", lib);
    }
    catch (const exception &e)
    {
        cout << "Error loading data: " << e.what() << endl;
        cout << "Starting with empty database..." << endl;
    }

    int choice;
    try
    {
        do
        {
            clearScreen();
            cout << "\n===== LIBRARY MANAGEMENT SYSTEM =====\n";
            cout << "1. Sign in\n";
            cout << "2. Sign up\n";
            cout << "3. View Available Resources (Guest)\n";
            cout << "0. Exit\n";

            choice = getValidInt("Enter choice: ");

            try
            {
                if (choice == 1)
                {
                    User *loggedUser = lib.loginUser();

                    if (loggedUser == nullptr)
                    {
                        pauseScreen();
                        continue;
                    }

                    if (loggedUser->getType() == "admin")
                    {
                        Admin *admin = dynamic_cast<Admin *>(loggedUser);
                        admin_menu(admin, lib);
                    }
                    else
                    {
                        library_menu(loggedUser, lib);
                    }
                }

                else if (choice == 2)
                {
                    clearScreen();
                    lib.registerUser();
                    pauseScreen();
                }

                else if (choice == 3)
                {
                    clearScreen();
                    lib.showAvailableResources();
                    pauseScreen();
                }

                else if (choice == 0)
                {
                    cout << "Exiting system...\n";
                }

                else
                {
                    cout << "Invalid menu choice! Please select 0-3." << endl;
                    pauseScreen();
                }
            }
            catch (const invalid_argument &e)
            {
                cout << "Input Error: " << e.what() << endl;
                pauseScreen();
            }
            catch (const runtime_error &e)
            {
                cout << "Runtime Error: " << e.what() << endl;
                pauseScreen();
            }
            catch (const exception &e)
            {
                cout << "Error: " << e.what() << endl;
                pauseScreen();
            }
            catch (...)
            {
                cout << "An unexpected error occurred." << endl;
                pauseScreen();
            }

        } while (choice != 0);
    }
    catch (const invalid_argument &e)
    {
        cout << "Input Error: " << e.what() << endl;
        pauseScreen();
    }
    catch (const runtime_error &e)
    {
        cout << "Runtime Error: " << e.what() << endl;
        pauseScreen();
    }
    catch (const exception &e)
    {
        cout << "Error: " << e.what() << endl;
        pauseScreen();
    }
    catch (...)
    {
        cout << "An unexpected error occurred." << endl;
        pauseScreen();
    }

    try
    {
        SaveUsersToFile("database/users.csv", lib.getUsers());
        SaveResourcesToFile("database/resources.csv", lib.getResources());
        SaveBorrowHistoryToFile("database/borrowHistory.csv", lib);
        cout << "Data saved successfully." << endl;
    }
    catch (const exception &e)
    {
        cout << "Error saving data: " << e.what() << endl;
    }

    return 0;
}