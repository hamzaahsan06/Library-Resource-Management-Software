#include "../include/Menu.h"
#include "../include/Utils.h"
#include "../include/Title.h"
#include "../include/Colors.h"
#include <iostream>

using namespace std;
using namespace Utils;

void runLibrarySystem(Library &lib)
{
    int choice;
    do
    {
        clearScreen();
        cout << COLOR_MAIN_HEADER << "\n===== LIBRARY MANAGEMENT SYSTEM =====" << RESET << "\n";
        cout << COLOR_MENU_OPTION << "1. Sign in" << RESET << "\n";
        cout << COLOR_MENU_OPTION << "2. Sign up" << RESET << "\n";
        cout << COLOR_MENU_OPTION << "3. View Available Resources (Guest)" << RESET << "\n";
        cout << COLOR_MENU_OPTION << "0. Exit" << RESET << "\n";
        try
        {
            choice = getValidInt(COLOR_INPUT_PROMPT "Enter choice: " RESET);

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
                    handleAdminMenu(dynamic_cast<Admin *>(loggedUser), lib);
                }
                else
                {
                    handleUserMenu(loggedUser, lib);
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
                cout << COLOR_PROCESS << "Exiting system..." << RESET << "\n";
            }
            else
            {
                cout << COLOR_ERROR << "Invalid menu choice! Please select 0-3." << RESET << endl;
                pauseScreen();
            }
        }
        catch (const exception &e)
        {
            cout << COLOR_ERROR << "Error: " << e.what() << RESET << endl;
            pauseScreen();
        }
    } while (choice != 0);
}

void handleAdminMenu(Admin *admin, Library &lib)
{
    int adminChoice;
    do
    {
        clearScreen();
        cout << "\n===== ADMIN MENU =====\n";
        cout << "1. Add Resource\n2. Delete Resource\n3. Update Resource\n4. Delete User\n";
        cout << "5. Search User\n6. View All Customers Report\n7. Issued Resources\n";
        cout << "8. Overdue Resources\n9. Statistics\n10. Export Report\n11. Collect donation\n";
        cout << "12. Fine Management\n0. Logout\n";

        adminChoice = getValidInt("Enter choice: ");

        switch (adminChoice)
        {
        case 1:
            clearScreen();
            admin->addResource(lib);
            pauseScreen();
            break;
        case 2:
            clearScreen();
            admin->deleteResource(lib);
            pauseScreen();
            break;
        case 3:
            clearScreen();
            admin->updateResource(lib);
            pauseScreen();
            break;
        case 4:
            clearScreen();
            admin->deleteUser(lib);
            pauseScreen();
            break;
        case 5:
            clearScreen();
            admin->searchUser(lib);
            pauseScreen();
            break;
        case 6:
            clearScreen();
            admin->printAllCustomersReport(lib);
            pauseScreen();
            break;
        case 7:
            clearScreen();
            admin->printIssuedResources(lib);
            pauseScreen();
            break;
        case 8:
            clearScreen();
            admin->printOverdueResources(lib);
            pauseScreen();
            break;
        case 9:
            clearScreen();
            admin->generateStats(lib);
            pauseScreen();
            break;
        case 10:
            clearScreen();
            admin->exportReports(lib, "report.txt");
            pauseScreen();
            break;
        case 11:
        {
            clearScreen();
            int donorID = getValidInt("Enter User ID of the donor: ");
            User *donor = nullptr;
            for (auto u : lib.getUsers())
            {
                if (u->getUserID() == donorID && !u->getIsDeleted())
                {
                    donor = u;
                    break;
                }
            }
            if (donor)
                admin->collectDonationFromUser(donor, lib);
            else
                cout << "Error: User with ID " << donorID << " not found." << endl;
            pauseScreen();
        }
        break;
        case 12:
            clearScreen();
            admin->fineManagement(lib);
            pauseScreen();
            break;
        case 0:
            cout << "Logging out..." << endl;
            break;
        default:
            cout << "Invalid choice! Please select 0-12." << endl;
            pauseScreen();
        }
    } while (adminChoice != 0);
}

void handleUserMenu(User *loggedUser, Library &lib)
{
    int userChoice;
    do
    {
        clearScreen();
        cout << "\n===== USER MENU =====\n";
        cout << "1. View Profile\n2. Change Password\n3. View Available Resources\n";
        cout << "4. Search Resources\n5. Borrow Resource\n6. Return Resource\n0. Logout\n";

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
                    cout << "Resource with ID " << id << " not found or is deleted." << endl;
            }
            else
                cout << "Error: Invalid resource ID!" << endl;
            pauseScreen();
        }
        break;
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
                    cout << "Resource with ID " << id << " not found." << endl;
            }
            else
                cout << "Error: Invalid resource ID!" << endl;
            pauseScreen();
        }
        break;
        case 0:
            cout << "Logging out..." << endl;
            break;
        default:
            cout << "Invalid choice! Please select 0-6." << endl;
            pauseScreen();
        }
    } while (userChoice != 0);
}