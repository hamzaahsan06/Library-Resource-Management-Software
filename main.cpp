#include <iostream>
#include <cstdlib>
#include "include/Library.h"
#include "include/admin.h"
#include "FileHandling/FileHandler.h"
#include "Include/utils.h"
using namespace std;
using namespace Utils;

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#endif
}

void pauseScreen()
{
    cout << "\nPress Enter to return to menu...";
    cin.ignore(1000, '\n'); // discard leftover '\n' from getValidInt
    cin.get();
}

int main()
{
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
                    int adminChoice;

                    do
                    {
                        clearScreen();
                        cout << "\n===== ADMIN MENU =====\n";
                        cout << "1. Add Resource\n";
                        cout << "2. Delete Resource\n";
                        cout << "3. Update Resource\n";
                        cout << "4. Delete User\n";
                        cout << "5. Search User\n";
                        cout << "6. View All Customers Report\n";
                        cout << "7. Issued Resources\n";
                        cout << "8. Overdue Resources\n";
                        cout << "9. Statistics\n";
                        cout << "10. Export Report\n";
                        cout << "0. Logout\n";

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
                        case 0:
                            cout << "Logging out..." << endl;
                            break;
                        default:
                            cout << "Invalid choice! Please select 0-10." << endl;
                            pauseScreen();
                            break;
                        }

                    } while (adminChoice != 0);
                }
                else
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
                            int id;
                            id = getValidInt("Enter Resource ID: ");
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
                            int id;
                            id = getValidInt("Enter Resource ID: ");
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
        catch (const exception &e)
        {
            cout << "Error: " << e.what() << endl;
            pauseScreen();
        }

    } while (choice != 0);

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