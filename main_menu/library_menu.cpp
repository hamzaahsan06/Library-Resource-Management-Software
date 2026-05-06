#include "library_menu.h"
#include "../include/utils.h"
#include "clearANDpause.h"
#include <iostream>

using namespace std;
using namespace Utils;

void library_menu(User *loggedUser, Library &lib)
{
    int userChoice;
    do
    {
        clearScreen();
        cout << "\n===== USER MENU =====\n";
        cout << "1. View Profile\n";
        cout << "2. Change Password\n";
        cout << "3. View Available Resources\n";
        cout << "4. Search Resources\n";
        cout << "5. Borrow Resource\n";
        cout << "6. Return Resource\n";
        cout << "0. Logout\n";

        userChoice = getValidInt("Enter choice: ");

        switch (userChoice)
        {
        case 1:
            clearScreen();
            lib.showUserProfile(loggedUser);
            pauseScreen();
            break;
        case 2:
            clearScreen();
            lib.changePassword(loggedUser);
            pauseScreen();
            break;
        case 3:
            clearScreen();
            lib.showAvailableResources();
            pauseScreen();
            break;
        case 4:
            clearScreen();
            lib.searchResources();
            pauseScreen();
            break;
        case 5:
        {
            clearScreen();
            int id = getValidInt("Enter Resource ID: ");
            if (id)
            {
                bool found = false;
                for (auto r : lib.getResources())
                {
                    if (r->getResourceID() == id && !r->getIsDeleted())
                    {
                        lib.borrowResource(loggedUser, r);
                        found = true;
                        break;
                    }
                }
                if (!found)
                {
                    cout << "Resource with ID " << id << " not found or is deleted." << endl;
                }
            }
            else
            {
                cout << "Error: Invalid resource ID!" << endl;
            }
            pauseScreen();
            break;
        }
        case 6:
        {
            clearScreen();
            int id = getValidInt("Enter Resource ID: ");
            if (id)
            {
                bool found = false;
                for (auto r : lib.getResources())
                {
                    if (r->getResourceID() == id)
                    {
                        lib.returnResource(loggedUser, r);
                        found = true;
                        break;
                    }
                }
                if (!found)
                {
                    cout << "Resource with ID " << id << " not found." << endl;
                }
            }
            else
            {
                cout << "Error: Invalid resource ID!" << endl;
            }
            pauseScreen();
            break;
        }
        case 0:
            cout << "Logging out..." << endl;
            break;
        default:
            cout << "Invalid choice! Please select 0-6." << endl;
            pauseScreen();
            break;
        }
    } while (userChoice != 0);
}